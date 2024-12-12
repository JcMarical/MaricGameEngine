
#include "cdpch.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


#include "CryDust/Core/Application.h"

#include "CryDust/Core/Input.h"

#include "CryDust/Events/ApplicationEvent.h"

#include "CryDust/Renderer/Renderer.h"
#include <glfw/glfw3.h>



 
namespace CryDust {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		//构造单例
		CORE_DEBUG_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;


		m_Window = Window::Create();
		m_Window->SetEventCallback(CD_BIND_EVENT_FN(Application::OnEvent));


		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	
	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}

	//---------LayerStack----------

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	//----------Event------------

	void Application:: OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(CD_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(CD_BIND_EVENT_FN(Application::OnWindowResize));

		CORE_DEBUG_TRACE("{0}",e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			//处理了就直接停止
			if (e.Handled)
			{
				break;
			}
			(*--it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			//循环绘制
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			//没有缩小化
			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			//IMGUI渲染
			
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			

			m_Window->OnUpdate();
		}
	}



	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}



}

