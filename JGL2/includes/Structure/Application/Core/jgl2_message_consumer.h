#pragma once

#include "structure/jgl2_singleton.h"
#include "structure/jgl2_pool.h"
#include "structure/jgl2_polymorphic_container.h"
#include "structure/jgl2_locked_queue.h"

namespace jgl
{
	class MessageConsumer
	{
	public:
		using MessagePool = jgl::Singleton<jgl::Pool<jgl::PolymorphicContainer>>;

	private:
		jgl::LockedQueue<jgl::PolymorphicContainer*>& _messageToTreat;

		virtual void _treatMessage(jgl::PolymorphicContainer* p_message) = 0;

	public:
		MessageConsumer(jgl::LockedQueue<jgl::PolymorphicContainer*>& p_messageToTreat) :
			_messageToTreat(p_messageToTreat)
		{

		}

		void update()
		{
			while (_messageToTreat.empty() == false)
			{
				jgl::PolymorphicContainer* message = _messageToTreat.pop_front();

				_treatMessage(message);

				MessagePool::instance()->release(message);
			}
		}
	};
}