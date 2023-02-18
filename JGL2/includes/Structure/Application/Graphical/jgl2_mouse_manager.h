#pragma once

#include "structure/Application/Core/jgl2_message_consumer.h"
#include "structure/jgl2_mouse.h"

namespace jgl
{
	class MouseManager : public MessageConsumer
	{
	private:
		Mouse* _mouse;

		void _treatMessage(jgl::PolymorphicContainer* p_message);
		void _onUpdate();

	public:
		MouseManager(jgl::LockedQueue<jgl::PolymorphicContainer*>& p_messageToTreat);
		const Mouse* mouse() const { return (_mouse); }
	};
}