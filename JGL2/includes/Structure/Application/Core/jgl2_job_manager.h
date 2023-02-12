#pragma once

#include "jgl2_includes.h"
#include "structure/jgl2_thread.h"

namespace jgl
{
	class JobManager
	{
	public:
		using Job = std::function<int()>;

	private:
		static void _executeFuncts(const std::vector<Job>& p_jobs);

		class Data
		{
		private:
			jgl::Thread* thread;
			std::vector<Job> jobs;

		public:
			Data();
			Data(std::string p_name);
			void addJob(Job p_job);
			void start();
			void stop();
		};

		std::map<std::string, Data*> _datas;
		std::vector<Job> _jobs;


		void _start();
		void _stop();

	public:
		Data* createThread(std::string p_threadName);

		void addJob(std::string p_threadName, Job p_job);
		void addJob(Data* p_thread, Job p_job);
		void addJob(Job p_job);

		void run();
	};
}