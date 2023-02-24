#pragma once

#include "structure/jgl2_singleton.h"
#include "structure/jgl2_pool.h"
#include "structure/jgl2_polymorphic_container.h"
#include "structure/jgl2_locked_queue.h"

namespace jgl
{
	namespace Abstract
	{
		namespace Application
		{
			namespace Utils
			{
				class MessageConsumer
				{
				public:
					using MessagePool = Singleton<Pool<PolymorphicContainer>>;

				private:
					LockedQueue<PolymorphicContainer*>& _messageToTreat;

					virtual void _treatMessage(PolymorphicContainer* p_message) = 0;
					virtual void _onUpdate()
					{

					}
				public:
					MessageConsumer(LockedQueue<PolymorphicContainer*>& p_messageToTreat) :
						_messageToTreat(p_messageToTreat)
					{

					}

					void update()
					{
						_onUpdate();
						while (_messageToTreat.empty() == false)
						{
							PolymorphicContainer* message = _messageToTreat.pop_front();

							_treatMessage(message);

							MessagePool::instance()->release(message);
						}
					}
				};

			}
		}
	}
}