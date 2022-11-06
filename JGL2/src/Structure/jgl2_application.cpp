#include "Structure/jgl2_application.h"

namespace jgl
{
	Application* Application::instance()
	{
		return (_instance);
	}

	const jgl::ULong& Application::time() const
	{
		return (_time);
	}

	const jgl::Vector2Int& Application::size() const
	{
		return (_context.size());
	}

	void Application::_setViewport(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size)
	{
		_context.setViewport(p_anchor.x(), p_anchor.y(), p_size.x(), p_size.y());
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

			_keyboard._updateState();
			_mouse._updateState();
		}
	}
	void Application::_runRender()
	{
		while (_running == true)
		{
			//jgl::cout << " ===== " << jgl::endl;
			_context.clear();

			_pullWinMessage();

			//jgl::cout << "Rendering widgets" << jgl::endl;
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
				_widgets[i]->update();
			}

			for (jgl::Size_t i = 0; i < _widgets.size(); i++)
			{
				_widgets[i]->render();
			}

			_context.render();

			_keyboard._updateState();
			_mouse._updateState();
		}
	}

	void Application::_run()
	{
		_updateThread = new jgl::Thread("Update thread", [&]() {
			_runUpdate();
			});
		_runRender();
	}

	Application::Application(std::string p_title, jgl::Vector2Int p_size, jgl::Color p_backgroundColor)
	{
		if (_instance != nullptr)
			throw std::runtime_error("Application already created");
		_instance = this;

		_messagePool.setDefaultObject(new jgl::PolymorphicContainer());
		_context.initialize(p_title, p_size, 4, 2);

		_context.setup(p_backgroundColor);

		_create2DColorShader();
		_create2DTextureShader();
		_create2DTextTextureShader();

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

	void Application::activateMultiThread()
	{
		_multithreaded = true;
	}

	void Application::deactivateMultiThread()
	{
		_multithreaded = true;
	}

	int Application::run()
	{
		try
		{
			_running = true;

			if (_multithreaded == true)
				_run();
			else
				_runMonoThread();
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