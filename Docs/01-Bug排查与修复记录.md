# Bug 排查与修复记录

> 日期：2026-08-15。基于提交 `0bc1b69`（更新Readme，技术展示）排查。
> 重点目标：鼠标悬停/拾取渲染物体时引擎崩溃。

## 一、鼠标悬停/拾取崩溃 —— 根因链分析

悬停拾取的完整链路如下：

```
视口内鼠标移动
  → EditorLayer::OnUpdate 读取 ID 附件 (Framebuffer::ReadPixel(1, x, y))
  → 以像素值构造 Entity((entt::entity)pixelData, scene)
  → 点击时 SceneHierarchyPanel::SetSelectedEntity → GetComponent<TagComponent>()
  → entt registry.get<T>(垃圾句柄) → 越界访问 → 崩溃
```

`Entity::operator bool()` 只判断句柄是否为 `entt::null`，**无法甄别垃圾句柄**。
一旦 ID 缓冲中的值不是合法实体（着色器缓存过期导致顶点属性错位、整型属性
被错误解释、场景切换后的陈旧句柄），后续 `GetComponent` 直接崩溃。
原代码中 Stats 面板的悬停显示被 `#if 0` 注释掉，正是这个崩溃的应急屏蔽。

## 二、已修复的问题

### 1. 拾取路径缺少实体有效性校验（崩溃主因）✅
- `CryDust/src/CryDust/Scene/Entity.h`
  - 新增 `Entity::IsValid()`：`m_Scene->m_Registry.valid(handle)` 校验句柄
    在注册表中真实存在（entt 的 `valid()` 对任意垃圾值安全）。
  - 清理 `GetComponent` 中无效的 try/catch（assert 宏不会抛异常，该代码是
    死代码且误导），改为 `IsValid()` + `HasComponent<T>()` 双重断言。
- `CryDust-Editor/src/EditorLayer.cpp`（约 142 行）
  - 拾取处先构造临时 `Entity`，`IsValid()` 通过才赋给 `m_HoveredEntity`，
    否则置空。悬停显示与点击选中两条路径同时受益。
  - 重新启用 Stats 面板中的悬停实体名显示（原 `#if 0` 屏蔽代码）。

### 2. 整型顶点属性用错 GL 接口（拾取数据错误的根因之一）✅
- `CryDust/src/Platform/OpenGL/OpenGLVertexArray.cpp`
  - `Int/Int2/Int3/Int4/Bool` 原使用 `glVertexAttribPointer`，GL 会把整型
    顶点数据做浮点归一化转换，`a_EntityID` 进入着色器后全是错误值。
    改为 `glVertexAttribIPointer`。
  - `Mat3/Mat4` 原误用 `glVertexAttribIPointer`（矩阵是浮点数据），
    改回 `glVertexAttribPointer`。
  - 这就是提交记录里"鼠标拾取bug（只有第一个正确显示纹理）"的同族问题。

### 3. 着色器缓存无失效机制 ✅
- `CryDust/src/Platform/OpenGL/OpenGLShader.cpp`
  - 原逻辑：缓存文件存在就直接用，与源文件内容无关。改动 .glsl 后必须手动
    删除 `assets/cache/shader/opengl` 全目录（用户备注"着色器编译要全删掉才行"）。
  - 新增 FNV-1a 哈希（`Utils::HashData`），每个缓存文件旁写 `.hash` 文件：
    Vulkan 阶段哈希 GLSL 源文本，OpenGL 阶段哈希上游 SPIR-V。
    源修改 → 哈希不匹配 → 自动重编译，旧缓存无需手动清理。

### 4. 批渲染缓冲溢出（随机崩溃/内存损坏）✅
- `CryDust/src/CryDust/Renderer/Renderer2D.cpp`
  - `DrawCircle`：批次溢出检查被注释掉（`TODO: implement for circles`），
    圆数量超过 20000 时顶点指针越界写堆内存。已补 `NextBatch()` 检查。
  - `DrawLine`：完全无溢出检查。已补（按 `MaxVertices`）。
  - `DrawString`：每个字符 4 顶点/6 索引，长文本可写穿 `TextVertexBuffer`。
    已在循环内补 `NextBatch()` 检查。
  - `Shutdown()` 只释放 `QuadVertexBufferBase`，Circle/Line/Text 三个缓冲
    泄漏。已补齐 `delete[]`。

### 5. 文本渲染空指针解引用 ✅
- `Renderer2D::DrawString`：
  - `font` / `GetMSDFData()` / 图集纹理为空时直接崩溃，已加提前返回。
  - `fontGeometry.getGlyph(' ')->getAdvance()` 对不含空格字形的字体是
    空指针解引用，已加回退（advance 取 0.5f）。

### 6. Quad 片元着色器调试残留 ✅
- `CryDust-Editor/assets/shaders/Renderer2D_Quad.glsl`
  - 片元末尾把颜色强制覆盖为 `vec4(normalizedEntityID, 0, 0, 1)`，
    所有精灵被染红。已删除。

## 三、环境修复（仓库完整性）

子模块未随主仓库拉取完整，已修复：

| 子模块 | 状态 | 处理 |
|---|---|---|
| `CryDust/vendor/glm` | git 仓库损坏（无提交） | 按记录提交 `33b4a62` 浅克隆重建 |
| `CryDust/vendor/Box2D` | 空目录 | 按记录提交 `d473e95` 浅克隆重建 |
| `CryDust/vendor/ImGuizmo` | 空目录 | 按记录提交 `218d60b` 浅克隆重建 |
| `CryDust/vendor/yaml-cpp` | 空目录 | 按记录提交 `b5b07c8` 浅克隆重建 |
| `CryDust/vendor/msdf-atlas-gen` | 空目录 | 按记录提交 `b5beb29` 重建，含嵌套 msdfgen + freetype |
| `CryDust/vendor/GLFW` | 目录缺失且未登记在 git 树中 | 浅克隆 TheCherno/glfw master |

修复后 `premake5 vs2022` 可正常生成工程。

## 四、建议的后续加固（未改动，供参考）

1. **Debug 构建开启 `CD_ENABLE_ASSERTS`**：当前断言体系完善但没确认默认开启，
   建议在 premake/CMake 的 Debug 配置中显式定义。
2. **`OpenGLFramebuffer::ReadPixel/ClearAttachment` 的 `CORE_DEBUG_ASSERT`
   在 Release 下为空**，建议在越界时返回安全值而不是未定义行为。
3. **`Scene::OnUpdateSimulation`** 中 `m_StepFrames-- > 0` 会使步进计数变负数，
   建议 `m_StepFrames > 0` 判断后再自减。
4. **统一源文件编码**：`Scene.cpp/Scene.h` 是 UTF-16(BOM)+GBK 注释，其余文件
   是 UTF-8。混合编码会让 grep/代码审查工具失效，建议全仓库统一 UTF-8(BOM)。
5. 运行库层面建议在编辑器增加"崩溃时 dump"（`MiniDumpWriteDump` +
   `SetUnhandledExceptionFilter`），比 try/catch 更能定位悬停崩溃这类问题。

## 五、验证情况

- ~~本机无 Vulkan SDK~~ **已安装 Vulkan SDK 1.4.357.0**（`C:\VulkanSDK\1.4.357.0`，
  安装器已自动配置 `VULKAN_SDK` 环境变量与 PATH）。
- **全量 Debug x64 构建通过**：CryDust.lib / CryDust-Editor.exe / Sandbox.exe
  均已产出到 `bin\Debug-windows-x86_64\`。

### 构建环境修复记录（2026-08-15 补充）

1. **Vulkan SDK 1.4+ 不再附带 debug 版 shaderc/SPIRV-Cross 库**
   （`shaderc_sharedd.lib`、`spirv-cross-cored/glsld.lib` 已取消），
   原 `Dependencies.lua` 的 Debug 链接项指向不存在的文件。
   处理：Debug 配置统一改用 **Release CRT**（`runtime "Debug"` → `"Release"`，
   保留调试符号与 `CD_DEBUG`），Debug/Release 均链接 release 版 shaderc 库。
   涉及全部 10 个 premake5.lua 与 Dependencies.lua。
2. **重新生成工程需在带 `VULKAN_SDK` 环境变量的终端中运行 premake**
   （安装 SDK 后已打开的 VS/终端不会自动获得新环境变量，需要重启）。
