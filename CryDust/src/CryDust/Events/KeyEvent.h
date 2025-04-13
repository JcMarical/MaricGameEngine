#pragma once

#include "CryDust/Events/Event.h"

#include "CryDust/Core/KeyCodes.h"
namespace CryDust
{
	/// <summary>
	/// 按键基类抽象事件，不能单独创建
	/// </summary>
	class  KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() const{ return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		//受保护的，所以不能在任何东西中创建，除了它的派生类
		KeyEvent(const KeyCode keycode)
			: m_KeyCode(keycode) {}

		KeyCode m_KeyCode;
	};

	/// <summary>
	/// 按键按下事件
	/// </summary>
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {
		}

		bool IsRepeat() const { return m_IsRepeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_IsRepeat;
	};

	/// <summary>
	/// 按键释放事件
	/// </summary>
	class  KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent (const KeyCode keycode)
			: KeyEvent(keycode) {}


		/// <summary>
		/// ToString函数还是要单独重载下的
		/// </summary>
		/// <returns></returns>
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent:" << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);

	};

	/// <summary>
	/// 
	/// </summary>
	class  KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyTyped)
	};
}
