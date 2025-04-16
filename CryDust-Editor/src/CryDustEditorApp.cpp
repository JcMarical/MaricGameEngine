#include <CryDust.h>
#include <CryDust/Core/EntryPoint.h>
#include "EditorLayer.h"
namespace CryDust {
	class CryDustEditor : public Application
	{
	public:
		CryDustEditor(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(new EditorLayer());
		}



	};
	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "=CryDustEditor";
		spec.CommandLineArgs = args;

		return new CryDustEditor(spec);
	}
}
