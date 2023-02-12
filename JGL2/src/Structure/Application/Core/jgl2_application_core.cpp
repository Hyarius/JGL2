#include "Structure/Application/Core/jgl2_application_core.h"

namespace jgl
{
	ApplicationCore::ApplicationCore()
	{
		_instance = this;
		_addJob([&]() {_updateTime(); return (0); });
	}
	
	void ApplicationCore::_updateTime()
	{
		auto epoch = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch();

		_time = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
	}

	void ApplicationCore::_addJob(Job p_job)
	{
		_jobManager.addJob(p_job);
	}

	void ApplicationCore::_addJob(std::string p_threadName, Job p_job)
	{
		_jobManager.addJob(p_threadName, p_job);
	}

	void ApplicationCore::_updateWidget()
	{
		_widgetManager.castUpdateMethod();
	}
	
	void ApplicationCore::_renderWidget()
	{
		_widgetManager.castRenderMethod();
	}

	int ApplicationCore::run()
	{
		_setupJobs();

		_running = true;
		_errorValue = 0;

		_jobManager.run();

		return (_errorValue);
	}

	void ApplicationCore::quit(int p_errorValue)
	{
		_errorValue = p_errorValue;
		_running = false;
	}
}