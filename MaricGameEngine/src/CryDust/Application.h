#pragma once


namespace CryDust {

	class __declspec(dllexport) Application
	{
	public:
		Application();
		virtual ~Application();


		void Run();
	};

	//To be defined in client
	Application* CreateApplication();

}