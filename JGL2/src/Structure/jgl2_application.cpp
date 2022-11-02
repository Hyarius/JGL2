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

	jgl::Vector2Int Application::size()
	{
		return (_context.size());
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
		while (_running == true)
		{
			_context.clear();

			_pullWinMessage();

			for (jgl::Size_t i = 0; i < _widgets.size(); i++)
			{
				_widgets[i]->render();
			}
		
			_context.render();
		}
	}

	void Application::_runMonoThread()
	{
		while (_running == true)
		{
			_context.clear();

			_pullWinMessage();
			_updateTime();
			_handleWinMessage();

			for (jgl::Size_t i = 0; i < _widgets.size(); i++)
			{
				_widgets[i]->render();
			}

			for (jgl::Size_t i = 0; i < _widgets.size(); i++)
			{
				_widgets[i]->update();
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

	Application::Application(std::string p_title, jgl::Vector2Int p_size)
	{
		if (_instance != nullptr)
			throw std::runtime_error("Application already created");
		_instance = this;

		_messagePool.setDefaultObject(new jgl::PolymorphicContainer());
		_context.initialize(p_title, p_size, 4, 2);

		_context.setup(jgl::Color(0, 0, 150));

		_create2DColorShader();
		_create2DTextureShader();

		//resize(p_size);
		_updateTime();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);
		glClearDepth(0.0f);

		//_context.clear();
	}

	void Application::resize(jgl::Vector2Int p_size)
	{
		_context.resize(p_size.x(), p_size.y());
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
		catch (std::exception& e)
		{
			jgl::cout << e.what() << jgl::endl;
			return (1);
		}

		return (0);
	}

	void Application::addShader(std::string p_shaderName, Shader* p_shader)
	{
		if (_shaders.count(p_shaderName) != 0)
			delete _shaders[p_shaderName];
		_shaders[p_shaderName] = p_shader;
	}
	
	Shader* Application::shader(std::string p_shaderName)
	{
		if (_shaders.count(p_shaderName) == 0)
			return (nullptr);
		return (_shaders[p_shaderName]);
	}
}