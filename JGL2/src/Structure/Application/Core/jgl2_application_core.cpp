#include "Structure/Application/Core/jgl2_application_core.h"

namespace jgl::Abstract::Application
{
	Core::Core()
	{
		_instance = this;
		_addJob([&]() {
				_updateTime();
				return (0);
			});
		_addJob([&]() {
				if (_fpsTimer.isRunning() == false)
				{
					jgl::Abstract::Application::Core::EventManager::instance()->notify(Event::OnSecondSwap);
					_fpsTimer.start();
				}
				return (0);
			});
		EventManager::instanciate();
	}
	
	void Core::_updateTime()
	{
		FILETIME ft;
		GetSystemTimeAsFileTime(&ft);
		ULONGLONG tmpTime = ((ULONGLONG)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
		tmpTime -= 116444736000000000ULL; // Convert to Unix epoch time
		tmpTime /= 10000; // Convert to milliseconds
	
		_time = tmpTime;
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