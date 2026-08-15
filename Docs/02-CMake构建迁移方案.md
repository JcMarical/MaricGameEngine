# Premake(Lua) → CMake 构建系统迁移方案

> 目标：用 CMake 完全替代 Premake5，保持三配置（Debug/Release/Dist）、
> 现有目录布局（`bin/`、`bin-int/`）与 C# 脚本工程可用。
> CMake 最低版本建议 **3.24+**（`target_precompile_headers`、
> `find_package(Vulkan)` 组件、C# 支持都需要较新版本）。

## 一、现状梳理

| 工程 | 类型 | 说明 |
|---|---|---|
| CryDust | C++17 静态库 | 引擎本体，PCH=`cdpch.h`，内嵌编译 stb_image/glm/ImGuizmo |
| CryDust-Editor | C++ 可执行 | 编辑器，链接 CryDust，依赖 mono 静态库、Vulkan SDK |
| CryDust-ScriptCore | C# SharedLib (.NET 4.7.2) | 脚本核心，输出到 `CryDust-Editor/Resources/Scripts` |
| Sandbox | C++ 可执行 | 示例游戏 |
| Box2D / GLFW / Glad / ImGui / yaml-cpp / msdf-atlas-gen(+msdfgen+freetype) | vendor 库 | 各自带 premake5.lua |
| entt / glm / spdlog / stb_image / filewatch | header-only | 仅需 include 路径 |
| mono | 预编译静态库 | `vendor/mono/lib/<cfg>/libmono-static-sgen.lib` |
| shaderc / SPIRV-Cross | Vulkan SDK 提供 | `%VULKAN_SDK%/Lib`，Debug 用 `*d.lib` |

Premake 特性 → CMake 对照：

| Premake | CMake |
|---|---|
| `workspace/configurations {Debug,Release,Dist}` | `CMAKE_CONFIGURATION_TYPES`（VS 生成器多配置） |
| `pchheader/pchsource` | `target_precompile_headers()` |
| `includedirs` | `target_include_directories()` |
| `links` | `target_link_libraries()` |
| `defines` + `filter "configurations:*"` | `target_compile_definitions(... $<CONFIG:Debug>)` |
| `flags { "NoPCH" }` per-file | `set_source_files_properties(... SKIP_PRECOMPILE_HEADERS ON)` |
| `flags { "MultiProcessorCompile" }` | `/MP`（VS 下 CMake 默认不加，需显式） |
| `outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"` | `RUNTIME_OUTPUT_DIRECTORY` 等 + `$<CONFIG>` |
| `os.getenv("VULKAN_SDK")` | `find_package(Vulkan)` 或 `$ENV{VULKAN_SDK}` |
| C# 工程 | `include_external_msproject()` 或 `CSharp` 语言支持 |

## 二、目标目录结构

```
MaricGameEngine/
├─ CMakeLists.txt                 # 顶层：工程、全局设置、子目录
├─ cmake/
│  ├─ Dependencies.cmake          # vendor 目标定义（统一入口）
│  ├─ CompilerSettings.cmake      # 警告、/MP、运行库、配置
│  └─ VulkanDeps.cmake            # Vulkan SDK + shaderc + SPIRV-Cross 导入库
├─ CryDust/CMakeLists.txt
├─ CryDust-Editor/CMakeLists.txt
├─ CryDust-ScriptCore/…（沿用 .csproj，由顶层挂接）
└─ SandBox/CMakeLists.txt
```

## 三、关键实现

### 3.1 顶层 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.24)
project(CryDustWorkspace LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 保留 Premake 的三配置；Dist 语义 = Release + CD_DIST
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DIST "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_DIST "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_DIST "${CMAKE_C_FLAGS_RELEASE}")

# 输出布局与 premake 保持一致：bin/<cfg>-windows-x86_64/<prj>
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/$<CONFIG>-windows-x86_64")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin-int/$<CONFIG>-windows-x86_64")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/$<CONFIG>-windows-x86_64")

if(MSVC)
    add_compile_options(/MP)                       # MultiProcessorCompile
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerSettings)
include(Dependencies)

add_subdirectory(CryDust)
add_subdirectory(CryDust-Editor)
add_subdirectory(SandBox)

# C# 脚本核心：继续用现有 .csproj，挂进解决方案并在编辑器构建前构建
include_external_msproject(CryDust-ScriptCore
    "${CMAKE_SOURCE_DIR}/CryDust-ScriptCore/CryDust-ScriptCore.csproj")
add_dependencies(CryDust-Editor CryDust-ScriptCore)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT CryDust-Editor)
```

### 3.2 cmake/Dependencies.cmake —— vendor 统一处理

原则：**自带 CMakeLists 的用 `add_subdirectory`，header-only 用 `INTERFACE` 库，
零构建文件的写 10 行以内的小目标**。Premake 时代维护在 `vendor/premake` 里的
胶水代码全部废弃。

```cmake
# --- header-only：INTERFACE 目标 ---
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE ${CMAKE_SOURCE_DIR}/CryDust/vendor/entt/include)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_SOURCE_DIR}/CryDust/vendor/glm)

add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ${CMAKE_SOURCE_DIR}/CryDust/vendor/spdlog/include)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${CMAKE_SOURCE_DIR}/CryDust/vendor/stb_image)

add_library(filewatch INTERFACE)
target_include_directories(filewatch INTERFACE ${CMAKE_SOURCE_DIR}/CryDust/vendor/filewatch)

# --- 自带 CMakeLists 的 vendor ---
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED  OFF CACHE BOOL "" FORCE)
add_subdirectory(CryDust/vendor/Box2D EXCLUDE_FROM_ALL)   # 目标名 box2d

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(CryDust/vendor/GLFW EXCLUDE_FROM_ALL)    # 目标名 glfw

set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
add_subdirectory(CryDust/vendor/yaml-cpp EXCLUDE_FROM_ALL) # 目标名 yaml-cpp

set(MSDF_ATLAS_BUILD_STANDALONE OFF CACHE BOOL "" FORCE)
set(MSDF_ATLAS_USE_VCPKG OFF CACHE BOOL "" FORCE)
set(MSDF_ATLAS_USE_SKIA OFF CACHE BOOL "" FORCE)
add_subdirectory(CryDust/vendor/msdf-atlas-gen EXCLUDE_FROM_ALL)

# --- Glad：源码只有一个 .c，直接建目标 ---
add_library(Glad STATIC CryDust/vendor/Glad/src/glad.c)
target_include_directories(Glad PUBLIC CryDust/vendor/Glad/include)

# --- Dear ImGui：无官方 CMake，手工列出 5 个源文件 ---
add_library(ImGui STATIC
    CryDust/vendor/imgui/imgui.cpp
    CryDust/vendor/imgui/imgui_demo.cpp
    CryDust/vendor/imgui/imgui_draw.cpp
    CryDust/vendor/imgui/imgui_tables.cpp
    CryDust/vendor/imgui/imgui_widgets.cpp)
target_include_directories(ImGui PUBLIC CryDust/vendor/imgui)
target_link_libraries(ImGui PUBLIC glfw Glad)   # 若用 imgui 的 glfw/gl 后端再补 imconfig

# --- mono：预编译静态库，按配置导入 ---
add_library(mono STATIC IMPORTED GLOBAL)
set_target_properties(mono PROPERTIES
    IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/CryDust/vendor/mono/lib/Debug/libmono-static-sgen.lib"
    IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/CryDust/vendor/mono/lib/Release/libmono-static-sgen.lib"
    IMPORTED_LOCATION_DIST    "${CMAKE_SOURCE_DIR}/CryDust/vendor/mono/lib/Release/libmono-static-sgen.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/CryDust/vendor/mono/include")
# mono 静态链接的 Windows 依赖
target_link_libraries(mono INTERFACE Ws2_32 Winmm Version Bcrypt)

include(VulkanDeps)   # shaderc / SPIRV-Cross / vulkan-1
```

### 3.3 cmake/VulkanDeps.cmake —— 替代 `os.getenv("VULKAN_SDK")`

```cmake
find_package(Vulkan REQUIRED)   # 依赖 VULKAN_SDK 环境变量，官方 FindVulkan 自动识别

# Vulkan SDK 附带的 shaderc / SPIRV-Cross 只有 lib，没有 CMake config，手工导入
function(import_vulkan_lib name debug_lib release_lib)
    add_library(${name} SHARED IMPORTED GLOBAL)
    set_target_properties(${name} PROPERTIES
        IMPORTED_LOCATION_DEBUG   "$ENV{VULKAN_SDK}/Lib/${debug_lib}"
        IMPORTED_LOCATION_RELEASE "$ENV{VULKAN_SDK}/Lib/${release_lib}"
        IMPORTED_LOCATION_DIST    "$ENV{VULKAN_SDK}/Lib/${release_lib}"
        IMPORTED_IMPLIB_DEBUG     "$ENV{VULKAN_SDK}/Lib/${debug_lib}"
        IMPORTED_IMPLIB_RELEASE   "$ENV{VULKAN_SDK}/Lib/${release_lib}"
        IMPORTED_IMPLIB_DIST      "$ENV{VULKAN_SDK}/Lib/${release_lib}"
        INTERFACE_INCLUDE_DIRECTORIES "$ENV{VULKAN_SDK}/Include")
endfunction()

import_vulkan_lib(shaderc_shared   shaderc_sharedd.lib        shaderc_shared.lib)
import_vulkan_lib(spirv_cross_core spirv-cross-cored.lib      spirv-cross-core.lib)
import_vulkan_lib(spirv_cross_glsl spirv-cross-glsld.lib      spirv-cross-glsl.lib)
```

### 3.4 CryDust/CMakeLists.txt

```cmake
file(GLOB_RECURSE CRYDUST_SOURCES CONFIGURE_DEPENDS
    "src/*.h" "src/*.cpp"
    "vendor/stb_image/*.h" "vendor/stb_image/*.cpp"
    "vendor/ImGuizmo/ImGuizmo.h" "vendor/ImGuizmo/ImGuizmo.cpp")

add_library(CryDust STATIC ${CRYDUST_SOURCES})

target_precompile_headers(CryDust PRIVATE src/cdpch.h)
# premake 的 flags { "NoPCH" } 对应：
set_source_files_properties(vendor/ImGuizmo/ImGuizmo.cpp
    PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

target_include_directories(CryDust PUBLIC src)
target_compile_definitions(CryDust PUBLIC GLFW_INCLUDE_NONE)
target_compile_definitions(CryDust PRIVATE
    $<$<CONFIG:Debug>:CD_DEBUG;CD_ENABLE_ASSERTS>
    $<$<CONFIG:Release>:CD_RELEASE>
    $<$<CONFIG:Dist>:CD_DIST>)

target_link_libraries(CryDust
    PUBLIC  entt glm spdlog stb_image filewatch
    PRIVATE box2d glfw Glad ImGui yaml-cpp
            msdf-atlas-gen mono
            shaderc_shared spirv_cross_core spirv_cross_glsl
            Vulkan::Vulkan
            opengl32)
```

### 3.5 CryDust-Editor / SandBox

```cmake
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
add_executable(CryDust-Editor ${EDITOR_SOURCES})
target_include_directories(CryDust-Editor PRIVATE src)
target_link_libraries(CryDust-Editor PRIVATE CryDust)

# 资源与运行依赖拷贝（替代 premake 时代的脚本手工步骤）
add_custom_command(TARGET CryDust-Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/CryDust-Editor/assets
            $<TARGET_FILE_DIR:CryDust-Editor>/assets)
```

### 3.6 C# 工程的两个选项

1. **`include_external_msproject`（推荐，迁移成本为零）**：继续用 premake 生成
   一次 `.csproj`（或直接提交一个手写 csproj），CMake 只负责挂接与构建顺序。
2. **CMake CSharp 语言**：`enable_language(CSharp)` + `add_library(... .cs)`，
   可彻底脱离 VS 工程文件，但对 .NET Framework 4.7.2 的老式引用支持一般，
   需要改 SDK-style csproj。可作为二期目标。

## 四、迁移步骤（建议按序执行，每步可独立验证）

1. **冻结基线**：确保 premake 生成 + 完整构建通过，记录 bin 产物清单。
2. **搭建骨架**：顶层 CMakeLists + CompilerSettings + header-only 目标，
   先用 `cmake -G "Visual Studio 17 2022" -B build-cmake` 能配置成功。
3. **逐个迁移 vendor**：每接一个库就在 CryDust 里链接验证。
   风险最高的是 msdf-atlas-gen（嵌套 msdfgen/freetype），其官方 CMakeLists
   自带，注意关掉 SKIA 与 standalone。
4. **迁移 CryDust 引擎库**，用 Sandbox 做最小链接验证。
5. **迁移 Editor**，接 mono 导入库与资源拷贝。
6. **挂接 C# 工程**，验证 `Resources/Scripts` 输出路径不变。
7. **三配置验证**：Debug/Release/Dist 全量构建 + 编辑器冒烟测试
   （打开工程、悬停拾取、Play/Stop、保存场景）。
8. **并行运行期**：保留 premake 文件 1~2 周作为回退，确认无回归后删除
   `premake5.lua`、`Dependencies.lua`、`vendor/premake`、`scripts/Win-GenProjects.bat`。

## 五、迁移收益与注意事项

收益：
- 配置即代码，跨 IDE（VS / CLion / VS Code）与 CI（直接 `cmake --build`）。
- vendor 更新只需 `git submodule update` + reconfigure，不再依赖 premake 胶水。
- 为后续 Linux/macOS 移植铺路（premake 工程目前是 Windows-only 假设）。

注意事项：
- `file(GLOB_RECURSE CONFIGURE_DEPENDS)` 新增文件需触发 reconfigure；
  严格主义者可改为显式源文件清单。
- Vulkan SDK 的 shaderc 是 DLL（shaderc_shared），别忘了 POST_BUILD 拷贝
  `$ENV{VULKAN_SDK}/Bin/shaderc_sharedd.dll` 到输出目录（premake 时代靠
  系统 PATH 或手工放置，CMake 化时应显式化）。
- Dist 配置在 VS 的"配置管理器"中首次出现时，记得为 C# 工程映射到 Release。
