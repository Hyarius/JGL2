#include "Structure/jgl2_graphical_application.h"

#include "Structure/jgl2_timer.h"

namespace jgl
{
	GraphicalApplication* GraphicalApplication::instance()
	{
		return (static_cast<GraphicalApplication*>(_instance));
	}

	const jgl::Vector2Int& GraphicalApplication::size() const
	{
		return (_context.size());
	}

	const Float GraphicalApplication::maxDepth() const
	{
		return (_maxDepth);
	}

	void GraphicalApplication::setMaxDepth(jgl::Float p_maxDepth)
	{
		_maxDepth = p_maxDepth;
	}

	void GraphicalApplication::_setViewport(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size)
	{
		_context.setViewport(p_anchor.x(), p_anchor.y(), p_size.x(), p_size.y());
	}


	void GraphicalApplication::hideCursor()
	{
		setCursorVisibleStatus(FALSE);
	}
	void GraphicalApplication::revealCursor()
	{
		setCursorVisibleStatus(TRUE);
	}
	void GraphicalApplication::setCursorVisibleStatus(jgl::Bool p_state)
	{
		if (_running == true)
			throw std::runtime_error("Setting cursor isn't accessible during program execution");
		_isCursorVisible = p_state;

		if (p_state == false)
			while (ShowCursor(p_state) >= 0);
		else 
			while (ShowCursor(p_state) < 0);
	}
	jgl::Bool GraphicalApplication::isCursorVisible()
	{
		return (_isCursorVisible);
	}

	void GraphicalApplication::_runUpdate()
	{
		_updateTime();

		jgl::ULong actualNbFrame = 0;
		jgl::ULong nextTime = _time;

		while (_running == true)
		{
			if (nextTime < _time)
			{
				_nbUpdateFrame = actualNbFrame * 4;
				nextTime = _time + 250;
				actualNbFrame = 0;
			}
			actualNbFrame++;

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
	void GraphicalApplication::_runRender()
	{
		jgl::ULong actualNbFrame = 0;
		jgl::ULong nextTime = _time;

		while (_running == true)
		{
			if (nextTime < _time)
			{
				_nbRenderFrame = actualNbFrame * 4;
				nextTime = _time + 250;
				actualNbFrame = 0;
			}
			actualNbFrame++;

			_context.clear();

			_pullWinMessage();

			for (jgl::Size_t i = 0; i < _widgets.size(); i++)
			{
				_widgets[i]->render();
			}
		
			_context.render();
		}
	}

	void GraphicalApplication::_runMonoThread()
	{

		jgl::ULong actualNbFrame = 0;
		jgl::ULong nextTime = _time;

		while (_running == true)
		{
			if (nextTime < _time)
			{
				_nbUpdateFrame = actualNbFrame * 4;
				_nbRenderFrame = actualNbFrame * 4;
				nextTime = _time + 250;
				actualNbFrame = 0;
			}
			actualNbFrame++;

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

	void GraphicalApplication::_run()
	{
		jgl::cout.setPrefix("Renderer thread");
		_updateThread = new jgl::Thread("Update thread", [&]() {
			_runUpdate();
			});
		_runRender();
		_updateThread->join();
	}

	GraphicalApplication::GraphicalApplication(std::string p_title, jgl::Vector2Int p_size, jgl::Color p_backgroundColor) : AbstractApplication()
	{

		srand(::time(NULL));

		_messagePool.setDefaultObject(new jgl::PolymorphicContainer());
		_context.initialize(p_title, p_size, 4, 2);

		_context.setup(p_backgroundColor);

		_create2DColorShader();
		_create2DTextureShader();
		_create2DTextTextureShader();

		_updateTime();
	}

	void GraphicalApplication::resize(jgl::Vector2Int p_size)
	{
		_context.resize(p_size.x(), p_size.y());
		for (jgl::Size_t i = 0; i < _widgets.size(); i++)
		{
			_widgets[i]->setGeometry(jgl::Vector2Int(0, 0), size());
		}
	}

	void GraphicalApplication::activateMultiThread()
	{
		_multithreaded = true;
	}

	void GraphicalApplication::deactivateMultiThread()
	{
		_multithreaded = false;
	}

	void GraphicalApplication::setDefaultFont(Font* p_defaultFont)
	{
		_defaultFont = p_defaultFont;
	}
	
	Font* GraphicalApplication::defaultFont() const
	{
		return (_defaultFont);
	}

	int GraphicalApplication::run()
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

	void GraphicalApplication::addShader(std::string p_shaderName, Shader* p_shader)
	{
		if (_shaders.count(p_shaderName) != 0)
			delete _shaders[p_shaderName];
		_shaders[p_shaderName] = p_shader;
	}
	
	Shader* GraphicalApplication::shader(std::string p_shaderName)
	{
		if (_shaders.count(p_shaderName) == 0)
			return (nullptr);
		return (_shaders[p_shaderName]);
	}
}