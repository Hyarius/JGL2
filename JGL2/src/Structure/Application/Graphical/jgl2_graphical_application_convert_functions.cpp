#include "Structure/Application/Graphical/jgl2_graphical_application.h"

namespace jgl::Application
{
	jgl::Vector2 Graphical::convertScreenToOpenGL(jgl::Vector2Int p_screenPos)
	{
		jgl::Float x = (static_cast<jgl::Float>(p_screenPos.x + _window.origin().x)) / static_cast<jgl::Float>(size().x / 2) - 1.0f;
		jgl::Float y = (static_cast<jgl::Float>(p_screenPos.y + _window.origin().y)) / static_cast<jgl::Float>(size().y / 2) - 1.0f;
		return (Vector2(x, -y));
	}

	jgl::Vector2Int Graphical::convertOpenGLToScreen(jgl::Vector2 p_openGLPos)
	{
		jgl::Float x = (p_openGLPos.x + 1.0f) * (_window.size().x / 2.0f);
		jgl::Float y = (p_openGLPos.y - 1.0f) * (_window.size().y / -2.0f);
		return (Vector2Int(x, y));
	}

	jgl::Float Graphical::convertDepthToOpenGL(jgl::Float p_depth)
	{
		return (1.0f - ((p_depth + 1) / jgl::Application::Module::Window::C_MAX_DEPTH));
	}
}