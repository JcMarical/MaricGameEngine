#include <CryDust.h>
#include <CryDust/Core/EntryPoint.h>
#include "EditorLayer.h"
namespace CryDust {
	class CryDustEditor : public Application
	{
	public:
		CryDustEditor()
			: Application("CryDust Editor")
		{
			PushLayer(new EditorLayer());
		}


		~CryDustEditor()
		{
		}
	};
	Application* CreateApplication()
	{
		return new CryDustEditor();
	}
}