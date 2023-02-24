#pragma once

#include "structure/Application/Core/jgl2_message_consumer.h"
#include "structure/jgl2_mouse.h"

namespace jgl
{
	namespace Application
	{
		namespace Module
		{
			class Mouse : public Abstract::Application::Utils::MessageConsumer
			{
			private:
				jgl::Mouse* _mouse;

				void _treatMessage(jgl::PolymorphicContainer* p_message);
				void _onUpdate();

			public:
				Mouse(jgl::LockedQueue<jgl::PolymorphicContainer*>& p_messageToTreat);
				const jgl::Mouse* mouse() const { return (_mouse); }
			};
		}
	}
}