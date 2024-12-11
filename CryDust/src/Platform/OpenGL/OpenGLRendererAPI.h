#pragma once

#include "CryDust/Renderer/RendererAPI.h"

namespace CryDust {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:

		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};


}