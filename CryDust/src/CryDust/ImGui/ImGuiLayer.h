#pragma once

#include "CryDust/Layer.h"

#include "CryDust/Events/ApplicationEvent.h"
#include "CryDust/Events/KeyEvent.h"
#include "CryDust/Events/MouseEvent.h"

namespace CryDust {

	class CRYDUST_API ImGuiLayer : public Layer
	{

	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		float m_Time = 0.0f;
	
	};


}