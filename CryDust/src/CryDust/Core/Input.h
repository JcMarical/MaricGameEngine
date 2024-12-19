
#pragma once


#include <glm/glm.hpp>

#include "CryDust/Core/KeyCodes.h"
#include "CryDust/Core/MouseCodes.h"

namespace CryDust {
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
