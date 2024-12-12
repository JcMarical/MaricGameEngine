#pragma once

#include "CryDust/Core/Layer.h"

#include "CryDust/Events/ApplicationEvent.h"
#include "CryDust/Events/KeyEvent.h"
#include "CryDust/Events/MouseEvent.h"

namespace CryDust {

	class CRYDUST_API ImGuiLayer : public Layer
	{

	public:
		ImGuiLayer();
		~ImGuiLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;


		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	
	};


}