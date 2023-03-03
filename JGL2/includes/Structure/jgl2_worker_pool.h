#pragma once

#include "jgl2_includes.h"
#include "structure/jgl2_thread.h"
#include "structure/jgl2_locked_queue.h"

namespace jgl
{
    class WorkerPool
    {
    private:
        bool _execute = true;
        const std::function<void()> _threadExecutiveFunct = [&]() {
            while (_execute == true)
            {
                if (_jobQueue.empty() == false)
                {
                    auto tmp = _jobQueue.pop_front();
                    tmp();
                }
            }
        };
        std::vector<jgl::Thread*> _threads;
        jgl::LockedQueue<std::function<void()>> _jobQueue;

    public:
        WorkerPool(std::string p_poolName, size_t p_nbWorker = 0)
        {
            if (p_nbWorker == 0)
                p_nbWorker = std::thread::hardware_concurrency() - 1;

            for (size_t i = 0; i < p_nbWorker; i++)
            {
                jgl::Thread* newThread = new jgl::Thread(jgl::Thread::LaunchMethod::Delayed, p_poolName + " " + std::to_string(i), _threadExecutiveFunct);
                _threads.push_back(newThread);
            }
        }

        ~WorkerPool()
        {
            stop();
            for (size_t i = 0; i < _threads.size(); i++)
            {
                delete _threads[i];
            }
        }

        void start()
        {
            _execute = true;
            for (size_t i = 0; i < _threads.size(); i++)
            {
                _threads[i]->start();
            }
        }

        void stop()
        {
            _execute = false;
            for (size_t i = 0; i < _threads.size(); i++)
            {
                _threads[i]->join();
            }
        }

        template <typename Func, typename... Args>
        void addJob(Func&& func, Args&&... args)
        {
            std::function<void()> funct = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
            _jobQueue.push_back(funct);
        }
    };
}