#include "Structure/Widget/jgl2_frame.h"
#include "jgl2_drawing_functions.h"

namespace jgl
{
	Bool Frame::_onUpdate()
	{
		return (false);
	}

	void Frame::_onRender()
	{
		_box.render();
		jgl::drawRectangleColor(jgl::Color(255, 255, 255), Vector2Int(0, 0), Vector2Int(10, 10), depth() + 1);
	}

	void Frame::_onGeometryChange()
	{
		_box.setGeometry(anchor(), size());
	}

	Frame::Frame(Widget* p_parent) : Widget(p_parent),
		_box(this)
	{
		_setViewportOffset(Vector2Int(5, 5), Vector2Int(10, 10));
		_box.setBorderSize(Vector2Int(5, 5));
	}
	void Frame::setColor(Color p_backgroundColor, Color p_frontgroundColor)
	{
		_box.setColor(p_backgroundColor, p_frontgroundColor);
	}
}