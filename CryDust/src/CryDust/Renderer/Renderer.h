#pragma once

#include "RenderCommand.h"

namespace CryDust {
	
	/// <summary>
	/// 枚举类切换渲染API
	/// </summary>

	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};


}