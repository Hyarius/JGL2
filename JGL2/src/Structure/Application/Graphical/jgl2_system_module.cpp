#include "Structure/Application/Graphical/jgl2_system_module.h"
#include "Structure/Application/Core/jgl2_application_core.h"

namespace jgl::Application::Module
{
	void System::_treatMessage(jgl::PolymorphicContainer* p_message)
	{
		jgl::UInt messageId;
		*p_message >> messageId;

		switch (messageId)
		{
			case WM_DESTROY:
			{
				jgl::Abstract::Application::Core::instance()->quit();
				break;
			}
		}
	}

	System::System(jgl::LockedQueue<jgl::PolymorphicContainer*>& p_messageToTreat) :
		MessageConsumer(p_messageToTreat)
	{

	}
}