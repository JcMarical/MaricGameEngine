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
	class  KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keycode,int repeatCount)
			: KeyEvent(keycode),m_RepeatCount(repeatCount) {}

		/// <summary>
		/// 按下不动后，会持续的检测重复触发次数（不确定要使用，但是先设置了）
		/// </summary>
		/// <returns></returns>
		uint16_t  GetRepeatCount() const { return m_RepeatCount; }

		/// <summary>
		/// ToString函数还是要单独重载下的
		/// </summary>
		/// <returns></returns>
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent:" << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);
	private:
		uint16_t m_RepeatCount;

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
