#include "cdpch.h"
#include "CryDust/Renderer/VertexArray.h"

#include "CryDust/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace CryDust {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_DEBUG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}

		CORE_DEBUG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}