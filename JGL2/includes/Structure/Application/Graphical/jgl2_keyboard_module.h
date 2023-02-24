#pragma once

#include "structure/Application/Core/jgl2_message_consumer.h"
#include "structure/jgl2_keyboard.h"

namespace jgl
{
	namespace Application
	{
		namespace Module
		{
			class Keyboard : public Abstract::Application::Utils::MessageConsumer
			{
			private:
				jgl::Keyboard* _keyboard;

				void _treatMessage(jgl::PolymorphicContainer* p_message);
				void _onUpdate();

			public:
				Keyboard(jgl::LockedQueue<jgl::PolymorphicContainer*>& p_messageToTreat);
				const jgl::Keyboard* keyboard() const { return (_keyboard); }
			};
		}
	}
}