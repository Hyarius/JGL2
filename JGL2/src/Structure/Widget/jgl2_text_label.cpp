#include "Structure/Widget/jgl2_text_label.h"

namespace jgl
{
	jgl::Bool TextLabel::_onUpdate()
	{
		return (false);
	}

	void TextLabel::_onGeometryChange()
	{
		_box.setGeometry(jgl::Vector2Int(0, 0), size());
		_label.setGeometry(_box.usableAnchor(), _box.usableSize());

		if (_definedTextSize != 0)
			_label.setTextSize(_definedTextSize);
	}

	void TextLabel::_onRender()
	{
		_box.render(depth());
		_label.render(depth() + 0.2f);
	}

	TextLabel::TextLabel(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{

	}

	void TextLabel::setDefinedTextSize(const jgl::Size_t& p_definedTextSize)
	{
		_definedTextSize = p_definedTextSize;
	}

	const jgl::Size_t TextLabel::definedTextSize()
	{
		return (_definedTextSize);
	}

	jgl::WidgetComponent::Box& TextLabel::box()
	{
		return (_box);
	}

	jgl::WidgetComponent::TextLabel& TextLabel::label()
	{
		return (_label);
	}
}