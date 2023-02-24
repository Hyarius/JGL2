#include "Structure/Application/Core/jgl2_job_module.h"
#include "Structure/Application/Core/jgl2_application_core.h"

namespace jgl::Application::Module
{
	void Job::_executeFuncts(const std::vector<JobFunct>& p_jobs)
	{
		for (size_t i = 0; i < p_jobs.size(); i++)
		{
			int errorCode = p_jobs[i]();
			if (errorCode != 0)
				jgl::Abstract::Application::Core::instance()->quit(errorCode);
		}
	}

	Job::Data::Data() :
		thread(nullptr),
		jobs()
	{

	}

	Job::Data::Data(std::string p_name)
	{
		thread = new jgl::Thread(jgl::Thread::LaunchMethod::Delayed, p_name,
			[&]() {
				while (jgl::Abstract::Application::Core::instance()->_running == true)
				{
					Job::_executeFuncts(jobs);
				}
			});
	}

	void Job::Data::addJob(JobFunct p_job)
	{
		jobs.push_back(p_job);
	}

	void Job::Data::start()
	{
		if (thread == nullptr)
			throw std::runtime_error("No thread created in the jobManager");
		thread->start();
	}
	void Job::Data::stop()
	{
		if (thread == nullptr)
			throw std::runtime_error("No thread created in the jobManager");
		thread->join();
	}

	Job::Data* Job::createThread(std::string p_threadName)
	{
		_datas[p_threadName] = new Data(p_threadName);
		return (_datas.at(p_threadName));
	}
	void Job::addJob(std::string p_threadName, JobFunct p_job)
	{
		if (_datas.count(p_threadName) == 0)
		{
			createThread(p_threadName);
		}
		addJob(_datas[p_threadName], p_job);
	}
	void Job::addJob(Data* p_thread, JobFunct p_job)
	{
		p_thread->addJob(p_job);
	}
	void Job::addJob(JobFunct p_job)
	{
		_jobs.push_back(p_job);
	}
	void Job::_start()
	{
		for (auto it : _datas)
		{
			it.second->start();
		}
		if (_datas.size() != 0)
			jgl::cout.setPrefix("MainThread");
		while (jgl::Abstract::Application::Core::instance()->_running == true)
		{
			Job::_executeFuncts(_jobs);
		}
	}
	void Job::_stop()
	{
		for (auto& it : _datas)
		{
			it.second->stop();
		}
	}
	void Job::run()
	{
		_start();
		_stop();
	}

}