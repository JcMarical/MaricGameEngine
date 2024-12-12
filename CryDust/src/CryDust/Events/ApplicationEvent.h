#pragma once
#include "CryDust/Events/Event.h"



namespace CryDust {

		/// <summary>
	/// 窗口关闭事件
	/// </summary>
	class  WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};



	/// <summary>
	/// 窗口大小变化事件
	/// </summary>
	class   WindowResizeEvent : public Event
	{
	public:


		WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width),m_Height(height)
		{

		}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		/// <summary>
		/// ToString函数还是要单独重载下的
		/// </summary>
		/// <returns></returns>
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent:" << m_Width << ", " << m_Height;
			return ss.str();
		}


		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	private:
		unsigned int m_Width, m_Height;
	};




	class  AppTickEvent : public Event
	{
	public:
		AppTickEvent()  = default;
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class  AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;		
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class  AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

}