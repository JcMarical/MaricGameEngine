#include "cdpch.h"
#include "CryDust/Renderer/RenderCommand.h"


namespace CryDust {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}