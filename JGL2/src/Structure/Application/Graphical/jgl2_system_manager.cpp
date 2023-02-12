#include "Structure/Application/Graphical/jgl2_system_manager.h"
#include "Structure/Application/Core/jgl2_application_core.h"

namespace jgl
{
	void SystemManager::_treatMessage(jgl::PolymorphicContainer* p_message)
	{
		jgl::UInt messageId;
		*p_message >> messageId;

		switch (messageId)
		{
			case WM_DESTROY:
			{
				jgl::ApplicationCore::instance()->quit();
				break;
			}
		}
	}

	SystemManager::SystemManager(jgl::LockedQueue<jgl::PolymorphicContainer*>& p_messageToTreat) :
		MessageConsumer(p_messageToTreat)
	{

	}
}