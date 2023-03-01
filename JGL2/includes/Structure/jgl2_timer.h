#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

namespace jgl
{
	class Timer
	{
	public:
		enum class State {
			Stopped,
			Timeout,
			Running
		};

	private:
		State _state;
		jgl::ULong _startingTime;
		jgl::ULong _endingTime;
		jgl::ULong _timerDuration;

	public:
		Timer(jgl::ULong p_timer_period);

		void setDuration(jgl::ULong p_timer_period);
		jgl::ULong elapsedTime();
		jgl::ULong duration() { return (_timerDuration); }
		jgl::Float percent();
		void start();
		void stop();
		State state();
		jgl::Bool isRunning();

	};

	std::string to_string(Timer::State p_state);
}