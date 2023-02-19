#include "Structure/Widget/jgl2_frame.h"

namespace jgl
{
	jgl::Bool Frame::_onUpdate()
	{
		return (false);
	}
	void Frame::_onRender()
	{
		_box.render();
	}
	void Frame::_onGeometryChange()
	{
		_box.setGeometry(anchor(), size());
		_box.setDepth(depth());
	}

	Frame::Frame(std::string p_name) : jgl::WidgetCore(p_name)
	{
		viewport()->setOffset(5, 10);
	}
}