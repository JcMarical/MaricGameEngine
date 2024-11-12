#include "Application.h"


#include "Events/ApplicationEvent.h"
#include "Log.h"


namespace CryDust {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);

		DEBUG_TRACE(e.ToString());


		while (true);
	}

}