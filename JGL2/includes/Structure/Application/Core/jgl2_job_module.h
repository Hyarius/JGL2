#pragma once

#include "jgl2_includes.h"
#include "structure/jgl2_thread.h"

namespace jgl
{
	namespace Application
	{
		namespace Module
		{
			class Job
			{
			public:
				using JobFunct = std::function<int()>;

			private:
				static void _executeFuncts(const std::vector<JobFunct>& p_jobs);

				class Data
				{
				private:
					Thread* thread;
					std::vector<JobFunct> jobs;

				public:
					Data();
					Data(std::string p_name);
					void addJob(JobFunct p_job);
					void start();
					void stop();
				};

				std::map<std::string, Data*> _datas;
				std::vector<JobFunct> _jobs;


				void _start();
				void _stop();

			public:
				Data* createThread(std::string p_threadName);

				void addJob(std::string p_threadName, JobFunct p_job);
				void addJob(Data* p_thread, JobFunct p_job);
				void addJob(JobFunct p_job);

				void run();
			};
		}
	}
}