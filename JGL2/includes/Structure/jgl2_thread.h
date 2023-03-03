#pragma once

#include "Structure/jgl2_iostream.h"

namespace jgl
{
    class Thread {
        friend class WorkerPool;

    public:
        enum class LaunchMethod { Delayed, Immediate };

    private:
        std::string _message;
        std::function<void()> _funct;
        std::thread _thread;
        std::promise<bool> _starterSignal;

        ~Thread() {
            join();
        }
    public:
        template <typename Func, typename... Args>
        Thread(std::string p_message, Func&& p_func, Args&&... p_args)
            : _message(std::move(p_message)),
            _funct(std::bind(std::forward<Func>(p_func), std::forward<Args>(p_args)...)),
            _starterSignal() {
            auto wrapper = [this]() {
                jgl::cout.setPrefix("\033[0;37m" + _message);
                jgl::cerr.setPrefix("\033[1;31m" + _message);
                _starterSignal.get_future().wait();
                _funct();
            };
            _thread = std::thread(wrapper);
        }

        template <typename Func, typename... Args>
        Thread(LaunchMethod p_launchMethod, std::string p_message, Func&& p_func, Args&&... p_args)
            : Thread(p_message, p_func, p_args...) {
            if (p_launchMethod == LaunchMethod::Immediate) {
                start();
            }
        }

        std::thread::id getId() const
        {
            return (_thread.get_id());
        }

        void join()
        {
            if (_thread.joinable())
                _thread.join();
        }

        void start() {
            _starterSignal.set_value(true);
        }
	};
}