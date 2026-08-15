# KlayGE（龚敏敏）学习借鉴分析

> KlayGE：龚敏敏 2003 年起开发的开源跨平台游戏引擎，
> 仓库 https://github.com/gongminmin/KlayGE ，约 6000+ 提交，国内最有影响力的
> 自研 C++ 引擎之一。
> **许可证警告：KlayGE 是 GPL-2.0**。可以学习其架构与设计思想，
> 但不能直接复制代码到闭源项目中，否则衍生作品必须 GPL 开源。

## 一、KlayGE 架构速览

| 维度 | KlayGE 的做法 |
|---|---|
| 构建 | CMake（≥3.16）+ Python 脚本（Build.py / DeployKlayGE.py / Package.py），Azure Pipelines CI |
| 渲染后端 | **插件式渲染引擎**：D3D11、D3D12、OpenGL、OpenGL ES 各自编成独立 DLL，运行时按平台加载 |
| 基础库 | KFL（KlayGE Foundation Library）：数学、容器、线程、工具与引擎解耦，可单独使用 |
| 着色器 | 自研 Effect 框架（.fxml 效果文件）+ FXMLJit 离线/JIT 编译；DXBC2GLSL 实现 D3D 字节码到 GLSL 的翻译，一份 shader 多后端复用 |
| GL 加载 | 自研 glloader（不依赖 glad/glew），按扩展/版本生成加载代码 |
| 字体 | 自研 kfont 格式 + 距离场渲染（与 CryDust 的 msdf 思路一致，但带了完整的离线制作工具链） |
| 资源 | 统一 ResLoader + 异步加载；Pak 打包部署 |
| 平台 | 开发机 Win/Linux/macOS；部署 Win 桌面/商店、Android、iOS、Linux、macOS |
| 渲染管线 | 延迟渲染（Deferred）+ PBR + 完整后处理链，常年跟进图形学前沿（GPGPU、次表面散射等示例） |

## 二、与 CryDust 现状的对比

| 维度 | CryDust 现状 | KlayGE | 差距判断 |
|---|---|---|---|
| 渲染 API 抽象 | `RendererAPI` 虚类 + OpenGL 单实现，但抽象层薄、OpenGL 假设散落各处 | 渲染引擎是运行时插件，引擎核心对 API 完全无感知 | 大 |
| 构建系统 | Premake(Lua)，Windows-only，Vulkan SDK 靠环境变量硬编码 | CMake + Python，跨平台 + CI | 大（见 02 号文档） |
| 着色器 | 每后端一份源码，运行期 shaderc 编译，缓存无失效机制（已修） | 统一效果描述 + 跨字节码翻译，离线编译 | 大 |
| 资源管理 | 直接文件路径读写 | 抽象 ResLoader + 打包 + 异步 | 中 |
| 字体 | msdf-atlas-gen 运行时加载 | kfont 离线工具链 + 运行时渲染 | 小 |
| 数学库 | glm | KFL 自带数学（与渲染深度整合） | 无（glm 够用） |
| 脚本 | C# (mono) | Python（工具链）/无游戏脚本 | 方向不同，CryDust 更现代 |

## 三、值得借鉴的具体内容（按投入产出排序）

### 1. 渲染插件化架构（最值得学的设计思想）
KlayGE 的 `RenderEngine` 是纯接口，D3D11/GL 等实现放在独立 DLL，
程序启动时按配置加载。CryDust 不必一步到位做 DLL 插件，但应把
`RendererAPI` 的"创建"收敛到单一工厂，并把 OpenGL 专属假设
（如 `OpenGLShader` 被直接 include、帧缓冲格式枚举）全部压进
`Platform/OpenGL` 目录——这是后续接 Vulkan/D3D 的前置条件。

### 2. 着色器跨后端复用：DXBC2GLSL 的思路
KlayGE 一份 HLSL 编译到 DXBC 再翻译成 GLSL，避免了"每个后端写一遍 shader"。
CryDust 已有 shaderc + SPIRV-Cross：完全可以反过来走 **GLSL → SPIR-V →
各后端语言**（SPIRV-Cross 支持 HLSL/MSL 输出），现在的代码只做了
GLSL→SPIR-V→GLSL 的自转。把 `m_OpenGLSPIRV` 那一步推广成
"按当前 RendererAPI 输出目标语言"，就是 KlayGE 效果的轻量实现。

### 3. CMake + Python 的构建分层
KlayGE 的 CMake 只管编译，Python 管编排（配置、部署、打包）。
CryDust 迁移 CMake 后，建议同样用 Python 做 Setup（检查 Vulkan SDK、
拉子模块、生成工程），替代现在的 `Setup.bat + Setup.py` 半吊子组合。

### 4. 离线资产工具链思想
kfont/FXMLJit/NormalMapCompressor 都是"离线重工具，运行时轻加载"。
CryDust 目前字体图集、shader 都是运行时处理，启动慢且出错只能在运行时发现。
可逐步增加：`xxx --import` 命令行导入器（纹理压缩、字体图集、shader 预编译），
运行时只读处理后的二进制。

### 5. CI 常态化
Azure Pipelines 每次提交全平台构建。CryDust 迁移 CMake 后接 GitHub Actions
成本极低（windows-latest + Vulkan SDK 缓存），能在合并前拦住"忘删 shader 缓存
才发现编译错"这类问题。

### 6. 基础库与引擎解耦（KFL）
KFL 可被任何项目单独使用。CryDust 的 Core（Log/Assert/UUID/Timer/Buffer）
也有这个潜质，拆成独立目标后能单测、能复用。

## 四、不建议照搬的

- **GPL 许可证**：任何代码级复用都会传染。只学设计。
- **自研数学库 / glloader / kfont**：重复造轮子，glm/glad/msdf 已够用。
- **D3D11 时代的延迟渲染框架**：2026 年直接学它的思路做 clustered forward+
  或 visibility buffer 更合适。
- **Effect/.fxml 体系**：重型 XML 描述，对 2D→3D 过渡期的 CryDust 过重，
  先做简单的 Material 抽象即可。

## 五、结论

KlayGE 对 CryDust 最大的价值不是某个模块，而是三点**工程文化**：
插件化渲染后端、离线工具链 + 轻运行时、CMake/CI 常态化。
这三点恰好对应本文档 02（CMake 迁移）、04（3D 升级中的渲染抽象）
与 05（改进建议）的核心方向。
