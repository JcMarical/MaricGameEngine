#include "cdpch.h"
#include "CryDust/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace CryDust {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    CORE_DEBUG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}
		CORE_DEBUG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
