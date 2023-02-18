#include "Structure/Application/Graphical/jgl2_keyboard_manager.h"
#include "jgl2_basic_functions.h"
namespace jgl
{
	void KeyboardManager::_treatMessage(jgl::PolymorphicContainer* p_message)
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

	KeyboardManager::KeyboardManager(jgl::LockedQueue<jgl::PolymorphicContainer*>& p_messageToTreat) :
		MessageConsumer(p_messageToTreat),
		_keyboard(new Keyboard())
	{

	}

	void KeyboardManager::_onUpdate()
	{
		_keyboard->_updateState();
	}
}