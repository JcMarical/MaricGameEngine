
#include "cdpch.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


#include "Application.h"

#include "Input.h"

#include "Events/ApplicationEvent.h"

#include "CryDust/Renderer/Renderer.h"
#include <glfw/glfw3.h>



#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
 
namespace CryDust {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		//构造单例
		CORE_DEBUG_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;


		m_Window = std::unique_ptr<Window> (Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));


		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	
	}

	Application::~Application()
	{

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
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
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

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

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




}