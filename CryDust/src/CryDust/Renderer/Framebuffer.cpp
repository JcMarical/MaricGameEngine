#include "cdpch.h"
#include "Framebuffer.h"
#include "CryDust/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
namespace CryDust {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_DEBUG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
		}
		CORE_DEBUG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}