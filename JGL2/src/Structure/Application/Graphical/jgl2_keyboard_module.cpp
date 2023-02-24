#include "Structure/Application/Graphical/jgl2_keyboard_module.h"
#include "jgl2_basic_functions.h"

namespace jgl::Application::Module
{
	void Keyboard::_treatMessage(jgl::PolymorphicContainer* p_message)
	{
		jgl::UInt messageId;
		*p_message >> messageId;

		switch (messageId)
		{
		case WM_CHAR:
		{
			jgl::UInt value;

			*p_message >> value;

			_keyboard->_entry = static_cast<jgl::UChar>(value);
			break;
		}
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			jgl::UInt value;

			*p_message >> value;

			_keyboard->_keys[value] = jgl::InputStatus::Pressed;

			break;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			jgl::UInt value;

			*p_message >> value;

			_keyboard->_keys[value] = jgl::InputStatus::Released;
			break;
		}
		}
	}

	Keyboard::Keyboard(jgl::LockedQueue<jgl::PolymorphicContainer*>& p_messageToTreat) :
		MessageConsumer(p_messageToTreat),
		_keyboard(new jgl::Keyboard())
	{

	}

	void Keyboard::_onUpdate()
	{
		_keyboard->_updateState();
	}
}