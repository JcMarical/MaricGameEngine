
#pragma once


#include "CryDust/Core/Base.h"

#include "CryDust/Core/KeyCodes.h"
#include "CryDust/Core/MouseCodes.h"

namespace CryDust {
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}