#include "jgl2.h"

namespace jgl
{
	Vector3 convert_screen_to_opengl(const Vector2Int source, jgl::Float level)
	{
		if (jgl::Application::active_application() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "jgl::Application not created");
		if (jgl::Application::active_application()->active_viewport() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "No active viewport");
		jgl::Float x = (source.x) / (jgl::Application::active_application()->active_viewport()->area().x / 2.0f) - 1.0f;
		jgl::Float y = -((source.y) / (jgl::Application::active_application()->active_viewport()->area().y / 2.0f) - 1.0f);
		jgl::Float ratio = 1;
		if (level != 0)
			ratio = 1.0f / level;
		return (Vector3(x, y, ratio));
	}

	Vector2Int convert_opengl_to_screen(const Vector2Int source)
	{
		if (jgl::Application::active_application() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "jgl::Application not created");
		if (jgl::Application::active_application()->active_viewport() == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "No active viewport");
		jgl::Float x = (source.x + 1.0f) * (jgl::Application::active_application()->active_viewport()->area().x / 2.0f);
		jgl::Float y = (source.y - 1.0f) * (jgl::Application::active_application()->active_viewport()->area().y / -2.0f);
		return (Vector2Int(x, y));
	}
}