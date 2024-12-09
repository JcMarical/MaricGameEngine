#pragma once

#include "Core.h"
#include "Core.h"
#include "Events/Event.h"
#include "LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

#include "CryDust/ImGui/ImGuiLayer.h"
#include "CryDust/Renderer/Shader.h"
#include "CryDust/Renderer/Buffer.h"
#include "CryDust/Renderer/VertexArray.h"
namespace CryDust {

	class CRYDUST_API Application
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


		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

	private:


		bool OnWindowClose(WindowCloseEvent& e);
	
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;

	};

	//To be defined in client
	Application* CreateApplication();

}