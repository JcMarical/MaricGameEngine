#include <CryDust.h>
#include <CryDust/Core/EntryPoint.h>
#include "EditorLayer.h"
namespace CryDust {
	class CryDustEditor : public Application
	{
	public:
		CryDustEditor(ApplicationCommandLineArgs args)
			: Application("CryDust Editor",args)
		{
			PushLayer(new EditorLayer());
		}


		~CryDustEditor()
		{
		}
	};
	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new CryDustEditor(args);
	}
}
