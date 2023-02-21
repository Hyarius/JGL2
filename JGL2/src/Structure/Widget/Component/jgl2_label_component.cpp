#include "Structure/Widget/Component/jgl2_label_component.h"

namespace jgl
{
jgl::Color LabelDefaultValues::textColor = jgl::Color(255, 255, 255);
jgl::Color LabelDefaultValues::outlineColor = jgl::Color(0, 0, 0);

Label::Label() :
	_textColor(LabelDefaultValues::textColor),
	_outlineColor(LabelDefaultValues::outlineColor),
	_font(jgl::Application::instance()->defaultFont()),
	_text("")
{

}

void Label::reset()
{
	_textColor.reset();
	_outlineColor.reset();
	_font.reset();
}

void Label::useValue()
{
	_textColor.useValue();
	_outlineColor.useValue();
	_font.useValue();
}

void Label::setText(std::string p_text)
{
	_text = p_text;
}

void Label::setTextSize(jgl::Size_t p_textSize)
{
	_textSize = p_textSize;
}

void Label::setOutlineSize(jgl::Size_t p_outlineSize)
{
	_outlineSize = p_outlineSize;
}

void Label::setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size)
{
	_anchor = p_anchor;
	_size = p_size;
}

void Label::setDepth(jgl::Float p_depth)
{
	_depth = p_depth;
}

void Label::render()
{
	_font.value()->draw(_text, _anchor, _textSize, _textColor, _outlineSize, _outlineColor, _depth);
}

}