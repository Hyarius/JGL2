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
		_label.setGeometry(_box.usableAnchor() + _label.labelOffset(), _box.usableSize() - _label.labelOffset() * jgl::Vector2Int(2, 2));

		if (_definedTextSize != 0)
			_label.setTextSize(_definedTextSize);
	}

	void TextLabel::_onRender()
	{
		_label.render(depth());
		_box.render(depth());
	}

	void TextLabel::_onPositionChange()
	{
		_box.recalc();
		_label.recalc();
	}

	TextLabel::TextLabel(std::string p_text, jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_label.setText(p_text);
	}

	void TextLabel::setDefinedTextSize(const jgl::Size_t& p_definedTextSize)
	{
		_definedTextSize = p_definedTextSize;
	}

	const jgl::Size_t& TextLabel::definedTextSize()
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