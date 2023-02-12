#include "Structure/jgl2_timer.h"
#include "Structure/Application/Graphical/jgl2_application.h"

namespace jgl
{
	Timer::Timer(jgl::ULong p_timer_period)
	{
		_state = State::Stopped;
		_timerDuration = p_timer_period;
	}

	void Timer::setDuration(jgl::ULong p_timer_period)
	{
		_state = State::Stopped;
		_timerDuration = p_timer_period;
	}

	void Timer::start()
	{
		_startingTime = jgl::Application::instance()->time();
		_state = State::Running;
	}

	void Timer::stop()
	{
		_state = State::Stopped;
	}

	Timer::State Timer::state()
	{
		if (_state == State::Running && _startingTime + _timerDuration <= jgl::Application::instance()->time())
		{
			_state = State::Timeout;
		}

		return (_state);
	}
	
	jgl::Bool Timer::isRunning()
	{
		Timer::State p_state = state();

		if (p_state == Timer::State::Running)
			return (true);
		return (false);
	}


	jgl::ULong Timer::elapsedTime()
	{
		return (jgl::Application::instance()->time() - _startingTime);
	}

	jgl::Float Timer::percent()
	{
		return (static_cast<jgl::Float>(elapsedTime()) / _timerDuration);
	}

	std::string to_string(Timer::State p_state)
	{
		switch (p_state)
		{
		case Timer::State::Running:
			return ("Running");
		case Timer::State::Stopped:
			return ("Stopped");
		case Timer::State::Timeout:
			return ("Timeout");
		}
		return ("Unknow");
	}
}