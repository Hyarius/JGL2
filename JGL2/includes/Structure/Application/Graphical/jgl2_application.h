#pragma once

#include "structure/jgl2_color.h"
#include "structure/jgl2_vector2.h"
#include "Structure/jgl2_font.h"
#include "structure/Application/Core/jgl2_application_core.h"
#include "structure/Application/Graphical/jgl2_window_manager.h"
#include "structure/Application/Graphical/jgl2_windows_api_manager.h"
#include "structure/Application/Graphical/jgl2_shader_atlas.h"
#include "structure/Application/Graphical/jgl2_keyboard_manager.h"
#include "structure/Application/Graphical/jgl2_mouse_manager.h"
#include "structure/Application/Graphical/jgl2_system_manager.h"

namespace jgl
{
	class Application : public jgl::ApplicationCore
	{
		friend class Viewport;
	public:
		static Application* instance() { return (static_cast<Application*>(_instance)); }

	private:
		WindowsAPIManager _APIManager;

		ShaderAtlas _shaderAtlas;

	public:
		WindowManager _windowManager;
		KeyboardManager _keyboardManager;
		MouseManager _mouseManager;
		SystemManager _systemManager;

		jgl::Font* _defaultFont;

		void _setupJobs();
		
		void _create2DColorShader();
		void _create2DTextureShader();
		void _create2DTextTextureShader();

	public:
		Application(std::string p_title, jgl::Vector2Int p_size, jgl::Color p_backgroundColor);

		const jgl::Vector2Int& size() const { return (_windowManager.size()); }
		ShaderAtlas& shaders() { return (_shaderAtlas); }

		void setDefaultFont(jgl::Font* p_defaultFont) { _defaultFont = p_defaultFont; }
		jgl::Font* const & defaultFont() const { return (_defaultFont); }
		const jgl::Keyboard* keyboard() const { return (_keyboardManager.keyboard()); }
		const jgl::Mouse* mouse() const { return (_mouseManager.mouse()); }

		jgl::Vector2 convertScreenToOpenGL(jgl::Vector2Int p_screenPos);
		jgl::Vector2Int convertOpenGLToScreen(jgl::Vector2 p_openGLPos);
		jgl::Float convertDepthToOpenGL(jgl::Float p_depth);
	};
}