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
		_box.render(depth());
	}

	void Frame::_onGeometryChange()
	{
		_box.setGeometry(Vector2Int(0, 0), size());
	}

	Frame::Frame(Widget* p_parent) : Widget(p_parent)
	{
		_setViewportOffset(Vector2Int(5, 5), Vector2Int(10, 10));
		_box.setBorderSize(Vector2Int(5, 5));
	}

	WidgetComponent::Box& Frame::box()
	{
		return (_box);
	}
	
	const WidgetComponent::Box& Frame::box() const
	{
		return (_box);
	}
}