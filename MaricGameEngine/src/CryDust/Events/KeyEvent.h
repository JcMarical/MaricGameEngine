#pragma once

#include "Event.h"


namespace CryDust
{
	/// <summary>
	/// 按键基类抽象事件，不能单独创建
	/// </summary>
	class CRYDUST_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const{ return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		int m_KeyCode;
		//受保护的，所以不能在任何东西中创建，除了它的派生类
		KeyEvent(int keycode) 
			: m_KeyCode(keycode){}
		
	};

	/// <summary>
	/// 按键按下事件
	/// </summary>
	class CRYDUST_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode,int repeatCount)
			: KeyEvent(keycode),m_RepeatCount(repeatCount) {}

		/// <summary>
		/// 按下不动后，会持续的检测重复触发次数（不确定要使用，但是先设置了）
		/// </summary>
		/// <returns></returns>
		inline int GetRepeatCount() const { return m_RepeatCount; }

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
		int m_RepeatCount;

	};

	/// <summary>
	/// 按键释放事件
	/// </summary>
	class CRYDUST_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent (int keycode)
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
}