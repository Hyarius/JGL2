#include "Structure/Application/Core/jgl2_application_core.h"

namespace jgl::Abstract::Application
{
	Core::Core()
	{
		_instance = this;
		_addJob([&]() {_updateTime(); return (0); });
	}
	
	void Core::_updateTime()
	{
		auto epoch = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch();

		_time = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
	}

	void Core::_addJob(Job p_job)
	{
		_jobs.addJob(p_job);
	}

	void Core::_addJob(std::string p_threadName, Job p_job)
	{
		_jobs.addJob(p_threadName, p_job);
	}

	void Core::_updateWidget()
	{
		_widgets.castUpdateMethod();
	}
	
	void Core::_renderWidget()
	{
		_widgets.castRenderMethod();
	}

	int Core::run()
	{
		_setupJobs();

		_running = true;
		_errorValue = 0;

		_jobs.run();

		return (_errorValue);
	}

	void Core::quit(int p_errorValue)
	{
		_errorValue = p_errorValue;
		_running = false;
	}
}