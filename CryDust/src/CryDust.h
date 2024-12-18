#pragma once

//交由应用程序使用

#include <stdio.h>
#include "CryDust/Core/Application.h"
#include "CryDust/Core/Layer.h"
#include "CryDust/Core/Log.h"

#include "CryDust/Core/Timestep.h"

#include "CryDust/Core/Input.h"
#include "CryDust/Core/KeyCodes.h"
#include "CryDust/Core/MouseCodes.h"



#include "CryDust/ImGui/ImGuiLayer.h"
//---------ECS----------
#include "CryDust/Scene/Scene.h"

#include "CryDust/Scene/Entity.h"
#include "CryDust/Scene/Components.h"
// ---Renderer------------------------

#include "CryDust/Renderer/Renderer.h"
#include "CryDust/Renderer/Renderer2D.h"
#include "CryDust/Renderer/RenderCommand.h"
		  
#include "CryDust/Renderer/Buffer.h"
#include "CryDust/Renderer/Shader.h"
#include "CryDust/Renderer/Framebuffer.h"
#include "CryDust/Renderer/Texture.h"
#include "CryDust/Renderer/VertexArray.h"
		  
#include "CryDust/Renderer/OrthographicCamera.h"
#include "CryDust/Renderer/OrthographicCameraController.h"
// -----------------------------------


//--------Entry Point----------

//#include "CryDust/Core/EntryPoint.h"

//-----------------------------