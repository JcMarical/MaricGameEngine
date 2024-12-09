#pragma once

namespace CryDust {
	
	/// <summary>
	/// 枚举类切换渲染API
	/// </summary>
	enum class RendererAPI
	{
		None = 0, OpenGL = 1
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};


}