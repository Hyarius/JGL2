#include "Structure/Application/Core/jgl2_job_manager.h"
#include "Structure/Application/Core/jgl2_application_core.h"

namespace jgl
{
	void JobManager::_executeFuncts(const std::vector<Job>& p_jobs)
	{
		for (size_t i = 0; i < p_jobs.size(); i++)
		{
			int errorCode = p_jobs[i]();
			if (errorCode != 0)
				ApplicationCore::instance()->quit(errorCode);
		}
	}

	JobManager::Data::Data() :
		thread(nullptr),
		jobs()
	{

	}

	JobManager::Data::Data(std::string p_name)
	{
		thread = new jgl::Thread(jgl::Thread::LaunchMethod::Delayed, p_name,
			[&]() {
				while (ApplicationCore::instance()->_running == true)
				{
					JobManager::_executeFuncts(jobs);
				}
			});
	}

	void JobManager::Data::addJob(Job p_job)
	{
		jobs.push_back(p_job);
	}

	void JobManager::Data::start()
	{
		if (thread == nullptr)
			throw std::runtime_error("No thread created in the jobManager");
		thread->start();
	}
	void JobManager::Data::stop()
	{
		if (thread == nullptr)
			throw std::runtime_error("No thread created in the jobManager");
		thread->join();
	}

	JobManager::Data* JobManager::createThread(std::string p_threadName)
	{
		_datas[p_threadName] = new Data(p_threadName);
		return (_datas.at(p_threadName));
	}
	void JobManager::addJob(std::string p_threadName, Job p_job)
	{
		if (_datas.count(p_threadName) == 0)
		{
			createThread(p_threadName);
		}
		addJob(_datas[p_threadName], p_job);
	}
	void JobManager::addJob(Data* p_thread, Job p_job)
	{
		p_thread->addJob(p_job);
	}
	void JobManager::addJob(Job p_job)
	{
		_jobs.push_back(p_job);
	}
	void JobManager::_start()
	{
		for (auto it : _datas)
		{
			it.second->start();
		}
		if (_datas.size() != 0)
			jgl::cout.setPrefix("MainThread");
		while (ApplicationCore::instance()->_running == true)
		{
			JobManager::_executeFuncts(_jobs);
		}
	}
	void JobManager::_stop()
	{
		for (auto& it : _datas)
		{
			it.second->stop();
		}
	}
	void JobManager::run()
	{
		_start();
		_stop();
	}

}