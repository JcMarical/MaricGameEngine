#pragma once
#include "CryDust/Core.h"

#include "CryDust/Events/Event.h"

namespace CryDust
{
	class CRYDUST_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		//链接
		virtual void OnAttach() {}
		//分离
		virtual void OnDetach() {}
		//每帧调用
		virtual void OnUpdate() {}
		//事件接收
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}