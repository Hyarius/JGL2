#pragma once

#include "structure/Application/Core/jgl2_message_consumer.h"

namespace jgl
{
	namespace Application
	{
		namespace Module
			{
				class System : public Abstract::Application::Utils::MessageConsumer
				{
				private:
					void _treatMessage(PolymorphicContainer* p_message);

				public:
					System(LockedQueue<PolymorphicContainer*>& p_messageToTreat);
				};
			}
	}
}