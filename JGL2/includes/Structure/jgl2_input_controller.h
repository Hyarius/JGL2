#pragma once

#include "jgl2_timer.h"
#include "jgl2_keyboard.h"
#include "jgl2_mouse.h"
#include "Structure/Application/Graphical/jgl2_graphical_application.h"

namespace jgl
{
	namespace Abstract
	{
		class InputController
		{
		protected:
			jgl::Timer _timer;
			std::function<void()> _funct;

			virtual jgl::Bool _needExecution() = 0;
			virtual jgl::Bool _skipCondition(){
				return (true);
			}

		public:
			template <typename Func, typename... Args>
			InputController(jgl::ULong p_inputDelay, Func&& p_func, Args&&... p_args) :
				_timer(p_inputDelay),
				_funct(std::bind(std::forward<Func>(p_func), std::forward<Args>(p_args)...))
			{

			}
			virtual void update()
			{
				if (_skipCondition() == true && _timer.isRunning())
					return;

				if (_needExecution() == true){
					_funct();
					_timer.start();
				}
			}
		};
	}

	class MouseInputController : public jgl::Abstract::InputController
	{
	private:
		jgl::Mouse::Button _button;
		jgl::InputStatus _expectedStatus;

		jgl::Bool _needExecution(){
			return (jgl::Application::Graphical::instance()->mouse()->getButton(_button) == _expectedStatus);
		}

	public:
		template <typename Func, typename... Args>
		MouseInputController(jgl::Mouse::Button p_button, jgl::InputStatus p_expectedStatus, jgl::ULong p_inputDelay, Func&& p_func, Args&&... p_args) :
			jgl::Abstract::InputController(p_inputDelay, std::forward<Func>(p_func), std::forward<Args>(p_args)...),
			_button(p_button),
			_expectedStatus(p_expectedStatus)
		{

		}
	};

	class KeyInputController : public jgl::Abstract::InputController
	{
	private:
		jgl::Keyboard::Key _key;
		jgl::InputStatus _expectedStatus;

		jgl::Bool _needExecution(){
			return (jgl::Application::Graphical::instance()->keyboard()->getKey(_key) == _expectedStatus);
		}

	public:
		template <typename Func, typename... Args>
		KeyInputController(jgl::Keyboard::Key p_key, jgl::InputStatus p_expectedStatus, jgl::ULong p_inputDelay, Func&& p_func, Args&&... p_args) :
			jgl::Abstract::InputController(p_inputDelay, std::forward<Func>(p_func), std::forward<Args>(p_args)...),
			_key(p_key),
			_expectedStatus(p_expectedStatus)
		{

		}
	};

	class EntryInputController : public jgl::Abstract::InputController
	{
	private:
		jgl::UChar _lastEntry = '\0';

		jgl::Bool _needExecution(){
			return (jgl::Application::Graphical::instance()->keyboard()->getEntry() >= ' ');
		}
		jgl::Bool _skipCondition(){
			return (_lastEntry == jgl::Application::Graphical::instance()->keyboard()->getEntry());
		}

	public:
		template <typename Func, typename... Args>
		EntryInputController(jgl::ULong p_inputDelay, Func&& p_func, Args&&... p_args) :
			jgl::Abstract::InputController(p_inputDelay, std::forward<Func>(p_func), std::forward<Args>(p_args)...)
		{

		}
	};
}