#pragma once

#include "CryDust/Core/Core.h"
#include "cdpch.h"


namespace CryDust{

	/// <summary>
	/// 事件类型
	/// </summary>
	enum class EventType {
		None = 0,

		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,

		AppTick,
		AppUpdate,
		AppRender,

		KeyPressed,
		KeyReleased,
		KeyTyped,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled

	};

	/// <summary>
	/// 事件分类
	/// </summary>
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

///定义获取事件类型的相关函数,便于快速重写
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
///定义获取事件种类的相关函数，便于快速重写
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

class  Event
{
	/// <summary>
	/// 友元：事件发送器
	/// </summary>
	friend class EventDispatcher;

public:
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetCategoryFlags() const = 0;
	virtual std::string ToString() const { return GetName(); }

	inline bool IsIncategory(EventCategory category)
	{
		return GetCategoryFlags() & category;
	}

	/// <summary>
/// 用来确定一个事件是否被处理了
/// </summary>
	bool Handled = false;

protected:

};


class EventDispatcher
{
	template<typename T>
	using EventFn = std::function<bool(T&)>;   //定义了一个类型别名EventFn，它是一个函数对象，接受一个T类型的引用，并返回一个布尔值。这个函数对象用于处理事件。
public:
	EventDispatcher(Event& event) : m_Event(event)
	{

	}


	/// <summary>
	/// 发送事件
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="func"></param>
	/// <returns></returns>
	template<typename T>
	bool Dispatch(EventFn<T> func)				
	{
		
		if (m_Event.GetEventType() == T::GetStaticType())
		{
			m_Event.Handled = func(*(T*)&m_Event); //如果事件类型匹配，那么将事件转换为T类型，并调用传入的处理函数func。处理函数的执行结果被存储在m_Event的m_Handled成员变量中。
			return true;
		}
		return false;
		
	}
private:
	Event& m_Event;
};

	/// <summary>
	/// 便于输出Event事件名 
	/// </summary>
	/// <param name="os"></param>
	/// <param name="e"></param>
	/// <returns></returns>
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}