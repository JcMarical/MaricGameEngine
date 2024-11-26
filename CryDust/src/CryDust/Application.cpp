
#include "cdpch.h"
#include "Application.h"

#include "Input.h"

#include "Events/ApplicationEvent.h"
#include <glad/glad.h>


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
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}


			m_Window->OnUpdate();
		}
	}



	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}




}