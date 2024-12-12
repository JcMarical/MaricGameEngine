#pragma once

#include "CryDust/Core/Core.h"
#include "CryDust/Events/Event.h"

#include "CryDust/Events/ApplicationEvent.h"
#include "CryDust/Core/Window.h"
#include "CryDust/Core/LayerStack.h"
#include "CryDust/ImGui/ImGuiLayer.h"
#include "CryDust/Renderer/Shader.h"
#include "CryDust/Renderer/Buffer.h"
#include "CryDust/Renderer/VertexArray.h"
#include "CryDust/Core/Timestep.h"

#include "CryDust/ImGui/ImGuiLayer.h"
namespace CryDust {

	class  Application
	{
	public:
		Application();
		virtual ~Application();
		void OnEvent(Event& e);
		void Run();


		void PushLayer(Layer* layer);
		void PushOverlay(Layer* later);


		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:


		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;

	};

	//To be defined in client
	Application* CreateApplication();

}