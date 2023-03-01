#pragma once

#include "structure/jgl2_color.h"
#include "structure/jgl2_vector2.h"
#include "Structure/jgl2_font.h"
#include "Structure/jgl2_timer.h"
#include "structure/Application/Core/jgl2_application_core.h"
#include "structure/Application/Graphical/jgl2_window_module.h"
#include "structure/Application/Graphical/jgl2_windows_api_module.h"
#include "structure/Application/Graphical/jgl2_shader_atlas.h"
#include "structure/Application/Graphical/jgl2_keyboard_module.h"
#include "structure/Application/Graphical/jgl2_mouse_module.h"
#include "structure/Application/Graphical/jgl2_system_module.h"
#include "structure/Application/Graphical/jgl2_application_event.h"

namespace jgl
{
	namespace Application
	{
		class Graphical : public Abstract::Application::Core
		{
			friend class Viewport;
			friend class ImageOutput;
		public:
			static Graphical* instance() { return (static_cast<Graphical*>(_instance)); }

			using EventManager = jgl::Singleton<jgl::Observer<Event>>;

		private:
			jgl::Application::Module::WindowsAPI _API;

			jgl::Application::Utils::ShaderAtlas _shader;

			jgl::Application::Module::Window _window;
			jgl::Application::Module::Keyboard _keyboard;
			jgl::Application::Module::Mouse _mouse;
			jgl::Application::Module::System _system;

			jgl::Font* _defaultFont;

			void _setupJobs();

			void _create2DColorShader();
			void _create2DTextureShader();
			void _create2DTextTextureShader();

			jgl::Timer _fpsTimer = jgl::Timer(1000);
			std::vector<std::function<void()>> _functToInvoke;

			jgl::Size_t _nbRender = 0;
			jgl::Size_t _renderPerSecond = 0;

			jgl::Size_t _nbUpdate = 0;
			jgl::Size_t _updatePerSecond = 0;

		public:
			Graphical(std::string p_title, jgl::Vector2Int p_size, jgl::Color p_backgroundColor);

			template <typename Func, typename... Args>
			void addFunctToInvokePerSecond(Func&& p_func, Args&&... p_args)
			{
				std::function<void()> funct = std::bind(std::forward<Func>(p_func), std::forward<Args>(p_args)...);
				_functToInvoke.push_back(funct);
			}

			const jgl::Size_t& nbRenderPerSecond() const { return (_renderPerSecond); }
			const jgl::Size_t& nbUpdatePerSecond() const { return (_updatePerSecond); }

			const jgl::Vector2Int& size() const { return (_window.size()); }
			jgl::Application::Utils::ShaderAtlas& shaders() { return (_shader); }

			void setDefaultFont(jgl::Font* p_defaultFont) { _defaultFont = p_defaultFont; }
			jgl::Font*& defaultFont() { return (_defaultFont); }
			jgl::Font* const& defaultFont() const { return (_defaultFont); }
			const jgl::Keyboard* keyboard() const { return (_keyboard.keyboard()); }
			const jgl::Mouse* mouse() const { return (_mouse.mouse()); }

			jgl::Vector2 convertScreenToOpenGL(jgl::Vector2Int p_screenPos);
			jgl::Vector2Int convertOpenGLToScreen(jgl::Vector2 p_openGLPos);
			jgl::Float convertDepthToOpenGL(jgl::Float p_depth);
		};
	}
}