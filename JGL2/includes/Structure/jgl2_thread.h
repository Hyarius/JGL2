#pragma once

#include "Structure/jgl2_iostream.h"

namespace jgl
{
	class Thread
	{
	private:
		std::thread* _thread;

		~Thread()
		{

		}

	public:
		template< class Function, class... Args>
		Thread(std::string p_name, Function&& p_funct, Args&&... p_argv)
		{
			_thread = new std::thread(
				[&](std::string p_name, Function&& p_funct, Args&&... p_argv)
				{
					cout = jgl::IOStream(p_name);
					p_funct(p_argv...);
				},
				p_name, p_funct, std::forward<Args>(p_argv)...);
		}

		std::thread::id getId() const;
		void join();
	};
}