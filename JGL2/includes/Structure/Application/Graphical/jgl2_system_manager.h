#pragma once

#include "structure/Application/Core/jgl2_message_consumer.h"

namespace jgl
{
	class SystemManager : public MessageConsumer
	{
	private:
		void _treatMessage(jgl::PolymorphicContainer* p_message);

	public:
		SystemManager(jgl::LockedQueue<jgl::PolymorphicContainer*>& p_messageToTreat);
	};
}