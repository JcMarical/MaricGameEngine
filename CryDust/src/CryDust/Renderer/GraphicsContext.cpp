#include "cdpch.h"
#include "CryDust/Renderer/GraphicsContext.h"
#include "CryDust/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"
namespace CryDust {
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_DEBUG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}
		CORE_DEBUG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}