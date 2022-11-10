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
		drawRectangleColor(_backgroundColor, Vector2Int(0, 0), size(), depth());
		drawRectangleColor(_frontgroundColor, anchorOffset(), size() - sizeOffset(), depth() + 0.1f);
	}

	void Frame::_onGeometryChange()
	{

	}

	Frame::Frame(Widget* p_parent) : Widget(p_parent)
	{
		_setViewportOffset(Vector2Int(5, 5), Vector2Int(10, 10));
	}
	void Frame::setColor(Color p_backgroundColor, Color p_frontgroundColor)
	{
		_frontgroundColor = p_frontgroundColor;
		_backgroundColor = p_backgroundColor;
	}
}