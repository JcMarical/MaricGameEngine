#pragma once

#include "CryDust/Core/Layer.h"

#include "CryDust/Events/ApplicationEvent.h"
#include "CryDust/Events/KeyEvent.h"
#include "CryDust/Events/MouseEvent.h"

namespace CryDust {

	class  ImGuiLayer : public Layer
	{

	public:
		ImGuiLayer();
		~ImGuiLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		
		void Begin();
		void End();


		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();

		uint32_t GetActiveWidgetID() const;
	private:
		bool m_BlockEvents = true;
	
	};


}
