#include "Structure/jgl2_graphical_application.h"

namespace jgl
{
	jgl::Vector2 GraphicalApplication::convertScreenToOpenGL(jgl::Vector2Int p_screenPos)
	{
		jgl::Float x = (static_cast<jgl::Float>(p_screenPos.x() - Viewport::C_ORIGIN.x())) / static_cast<jgl::Float>(size().x() / 2) - 1.0f;
		jgl::Float y = (static_cast<jgl::Float>(p_screenPos.y() - Viewport::C_ORIGIN.y())) / static_cast<jgl::Float>(size().y() / 2) - 1.0f;
		return (Vector2(x, -y));
	}
	
	jgl::Vector2Int GraphicalApplication::convertOpenGLToScreen(jgl::Vector2 p_openGLPos)
	{
		jgl::Float x = (p_openGLPos.x() + 1.0f) * (size().x() / 2.0f);
		jgl::Float y = (p_openGLPos.y() - 1.0f) * (size().y() / -2.0f);
		return (Vector2Int(x, y));
	}

	jgl::Float GraphicalApplication::convertDepthToOpenGL(jgl::Float p_depth)
	{
		return (1.0f - ((p_depth + 1) / jgl::GraphicalApplication::instance()->maxDepth()));
	}
}
