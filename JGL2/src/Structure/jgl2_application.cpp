#include "Structure/jgl2_application.h"

namespace jgl
{
	Application* Application::instance()
	{
		return (_instance);
	}

	jgl::ULong Application::time()
	{
		return (_time);
	}

	void Application::_updateTime()
	{
		auto epoch = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch();

		_time = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
	}

	void Application::_runUpdate()
	{
		_updateTime();

		jgl::ULong next_time = _time + 1000;

		while (_running == true)
		{
			_updateTime();
			_handleWinMessage();

			for (jgl::Size_t i = 0; i < _widgets.size(); i++)
			{
				_widgets[i]->update();
			}
		}
	}
	void Application::_runRender()
	{
		jgl::ULong next_time = _time + 1000;

		while (_running == true)
		{
			_pullWinMessage();

			_context.clear();

			for (jgl::Size_t i = 0; i < _widgets.size(); i++)
			{
				_widgets[i]->render();
			}
		
			_context.render();
		}
	}

	void Application::_run()
	{
		_running = true;

		_update_thread = new jgl::Thread("Update thread", [&]() {
				_runUpdate();
			});
		_runRender();
	}

	Application::Application(jgl::String p_title, jgl::Vector2Int p_size)
	{
		if (_instance != nullptr)
			throw jgl::Exception(1, "Application already created");
		_instance = this;

		_messagePool.setDefaultObject(new jgl::PolymorphicContainer());
		_context.initialize(p_title, p_size);

		_updateTime();
	}

	void Application::quit()
	{
		jgl::cout << "Quitting application !" << jgl::endl;
		_running = false;
	}

	int Application::run()
	{
		try
		{
			_run();
		}
		catch (jgl::Exception& e)
		{
			jgl::cout << e.what() << jgl::endl;
			return (e.errorId());
		}

		return (0);
	}

}