
#include "cdpch.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


#include "CryDust/Core/Application.h"



#include "CryDust/Events/ApplicationEvent.h"

#include "CryDust/Renderer/Renderer.h"

#include "CryDust/Scripting/ScriptEngine.h"

#include "CryDust/Core/Input.h"
#include "CryDust/Utils/PlatformUtils.h"



 
namespace CryDust {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{

		CD_PROFILE_FUNCTION();
		//构造单例
		CORE_DEBUG_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;


		// Set working directory here
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		m_Window = Window::Create(WindowProps(m_Specification.Name));

		m_Window->SetEventCallback(CD_BIND_EVENT_FN(Application::OnEvent));


		Renderer::Init();
		ScriptEngine::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	
	}

	Application::~Application()
	{

		CD_PROFILE_FUNCTION();

		ScriptEngine::Shutdown();
		Renderer::Shutdown();
	}

	//---------LayerStack----------

	void Application::PushLayer(Layer* layer)
	{

		CD_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{

		CD_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}


	void Application::Close()
	{
		m_Running = false;
	}

	//----------Event------------

	void Application:: OnEvent(Event& e)
	{

		CD_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(CD_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(CD_BIND_EVENT_FN(Application::OnWindowResize));

		CORE_DEBUG_TRACE("{0}",e.ToString());

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend();++it)
		{
			//处理了就直接停止
			(*it)->OnEvent(e);
			if (e.Handled)
			{
				break;
			}

		}
	}

	void Application::Run()
	{

		CD_PROFILE_FUNCTION();
		while (m_Running)
		{

			CD_PROFILE_FUNCTION();
			//循环绘制
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			float time = Time::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			//没有缩小化
			if (!m_Minimized)
			{

				CD_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);


				//IMGUI渲染

				m_ImGuiLayer->Begin();

				{
					CD_PROFILE_SCOPE("LayerStack OnImGuiRender");
					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();

				}

				m_ImGuiLayer->End();

			}
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
		CD_PROFILE_FUNCTION();
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

