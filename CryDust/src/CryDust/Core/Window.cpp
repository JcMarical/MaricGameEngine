#include "cdpch.h"
#include "CryDust/Core/Window.h"

#ifdef CD_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace CryDust
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef CD_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		CD_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}
