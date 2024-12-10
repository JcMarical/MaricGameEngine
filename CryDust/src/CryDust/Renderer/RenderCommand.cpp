#include "cdpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace CryDust {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}