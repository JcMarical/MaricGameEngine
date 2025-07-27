# CRYDUST ENGINE
 基于学习目的，制作的一个以OpenGL/C++为基础的游戏引擎，并结合Mono实现C#/C++相互调用。

# 引擎主要架构

## 一 引擎层

- CORE：

  - Application引擎应用运行基本逻辑：Layer层栈分层执行，主线程事件处理。
  - EntryPoint程序入口：日志初始化+主循环逻辑+性能分析会话插桩
  - FileSystem文件系统：文件缓冲+读取
  - Log日志系统：封装spdlog第三方库
  - 平台监测：利用宏检测平台触发条件编译（目前只做了Windows）
  - Timer计时器
  - UUID全局唯一ID映射表：利用库实现实体ID分配
  - Window：窗口处理

- Debug工具：

  - 性能分析工具：代码插桩记录函数运行消耗，序列化json放到Chrome可视化

- 事件处理Event：枚举分类+bool返回值函数指针，主要做按键处理

- ImGUi:编辑器窗口初始化与封装

- Math：主要还是glm库做封装

- Physics物理：给物理库Box2D库做封装。

- Project项目设置：

  - Project构成：项目名字+初始场景文件路径+资产文件路径+脚本模块路径

  - Project序列化封装

- **Renderer渲染**：准确来说是RHI，具体实现会交给平台层的OpenGL处理。方便以后拓展，目前只做了OpenGL暂时拿着个来举例。

  - Buffer：（顶点缓冲和索引）VBO和EBO的绑定，设置与发送
  - VertexArray：（顶点数组）VAO的绑定，设置与发送
  - Camera：相机需要发送的数据--比如投影矩阵设置
  - EditorCamera：相机基础设置（Transform，投影方式，渲染和更新）
  - CameraController：相机控制器
  - Font字体渲染：MSDF库的封装，本质为纹理绘制

  - FrameBuffer：帧缓冲的绑定，附着与读取等功能
  - GraphicsContext：渲染上下文，如初始化和双缓冲功能
  - RendererAPI渲染API：用于切换API设置，封装不同API的一些渲染接口，如数据准备，重置，绘制指令（DrawCall）等。
  - RenderCommand渲染指令：提供封装全局调用RendererAPI的一些功能
  - Renderer渲染器：统一更新，渲染主要逻辑
  - Renderer2D渲染器：各种2D的Quad，Texture，Line绘制
  - UniformBuffer：常量缓冲内存设置。
  - Shader：着色器绑定、创建，常量缓冲设置。以及加载读取和着色器编译。
  - Texture：提供纹理绑定，设置纹理描述符、以及纹理数据设置。

- **Scene场景**：

  - ECS：Entity-Component-System，基于entt库实现
    - Entity：entt实体
    - Component：ID、Tag为必须组件，此外有物理、渲染器、摄像机、脚本等一系列的组件实现。
    - System：其实这块没封装的特别细，用的时候直接拿到实体for循环调用
  - Scene：场景复制拷贝，ECS实体组件管理，相机设置，生命周期管理。
  - SceneCamera：正交、透视、投影矩阵的计算。
  - SceneSerialize场景序列化：利用YAML库，UUID与信息存储为Node
  - ScriptableEntity：C++原生脚本，生命周期函数注册到对应事件绑定器执行。

- **ScriptingC#与C++交互编程**：

  - ScriptGlue：通过拿到C#程序集，注册能调用的C++静态函数与组件，使得C#能够调用C++的函数。
  - ScriptEngine：提供RootDomain和AppDomain，加载核心程序集和应用程序集。获取C#的元信息，实现C++Invoke调用C#。并提供热重载功能。

- Utils：平台工具，基类，具体交给平台实现，比如打开文件和保存文件等。

## 二 平台层

主要做平台差异化开发。

- Window微软平台：窗口API，平台工具、输入检测

- OpenGL：对接引擎层中Renderer的具体图形API实现。

  

##  三 C#脚本层

### 核心程序集

该程序集输出为dll时将作为CoreAssembly供C++链接。

- InternalCalls：通过Internal、extern、static的修饰和InternalCall的函数定义声明，交给C++去具体实现，供其他程序集调用底层C++非托管代码。
- 核心组件：Entity、Component、KeyCode、Input等，本质上也是通过InteralCalls获取底层C++返回的值，Vector除外。

### 应用程序集

该程序集引用CoreAssembly，输出为dll时将作为AppAssembly供C++链接。

- 主要就是自己具体的功能实现，这块就和Unity没什么差别了。





## 四 工具层（编辑器层）

这块和Unity的具体使用也没有特别大的区别，做起来也稍微熟悉一些。

- SandboxProject：
  - Assets文件夹，存储纹理、场景、脚本等各种文件
- 一些Panel：ImGui写的界面，主要由内容浏览ContentBrowser界面，场景实体管理Hierachy界面，Inspector组件查看界面，顶部文件栏项目、场景加载保存。
- EditorLayer：场景渲染的核心
  - 各种初始化：相机、纹理、帧缓冲
  - 各种更新：帧缓冲，运行模式
  - **鼠标拾取**：场景交互的关键！纹理拾取读取实体后，更改上下文成为当前已选择实体。
  - 引用前面各种的各种Panel
  - OnOverlayRender：实体选取后，线框绘制并显示高亮（线框）、显示gizmos（以及碰撞体）、组件等信息。







# 引擎开发规范

## 1.头文件排列

- 预编译头（`.pch`）
- 自定义头文件（`"*.h"/ "*.hpp"`）
- 外部库文件/标准库文件（`<*>`）

