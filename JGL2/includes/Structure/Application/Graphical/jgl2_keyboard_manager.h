#pragma once

#include "structure/Application/Core/jgl2_message_consumer.h"
#include "structure/jgl2_keyboard.h"

namespace jgl
{
	class KeyboardManager : public MessageConsumer
	{
	private:
		Keyboard* _keyboard;

		void _treatMessage(jgl::PolymorphicContainer* p_message);
		void _onUpdate();

	public:
		KeyboardManager(jgl::LockedQueue<jgl::PolymorphicContainer*>& p_messageToTreat);
		const Keyboard* keyboard() const { return (_keyboard); }
	};
}