#include "Structure/jgl2_thread.h"

namespace jgl
{

	std::thread::id Thread::getId() const {
		return _thread->get_id();
	}

	void Thread::join() {
		if (_thread->joinable())
			_thread->join();
	}
}