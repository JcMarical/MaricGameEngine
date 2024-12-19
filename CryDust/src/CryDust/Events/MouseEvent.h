#pragma once

#include "CryDust/Events/Event.h"
#include "CryDust/Core/MouseCodes.h"
namespace CryDust {

	class  MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const MouseCode x,const MouseCode y)
			: m_MouseX(x), m_MouseY(y){}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << "," << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_MouseX, m_MouseY;
	};


	class  MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const MouseCode xOffset, const MouseCode yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << GetXOffset() << "," << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_XOffset, m_YOffset;
	};



	/// <summary>
	/// 鼠标按键基类
	/// </summary>
	class  MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseButtonEvent(const MouseCode button)
			: m_Button(button) {}

		int m_Button;
	};



	class  MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const MouseCode button)
			: MouseButtonEvent(button){}


		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoMouseButtonPressedEventvedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class  MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}


		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}