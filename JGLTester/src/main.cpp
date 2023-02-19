#include "jgl2.h"

struct LabelDefaultValues
{
public:
	static jgl::Color textColor;
	static jgl::Color outlineColor;
};

class Label
{
private:
	std::string _text;

	jgl::MonitoredValue<jgl::Font*> _font;
	jgl::MonitoredValue<jgl::Color> _textColor;
	jgl::MonitoredValue<jgl::Color> _outlineColor;

	jgl::Vector2Int _anchor;
	jgl::Vector2Int _size;
	jgl::Float _depth;

public:
	Label();
	void reset();
	void useValue();
	void setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size);
	void setDepth(jgl::Float p_depth);
	void render();
};

jgl::Color LabelDefaultValues::textColor = jgl::Color(255, 255, 255);
jgl::Color LabelDefaultValues::outlineColor = jgl::Color(0, 0, 0);

Label::Label() :
	_textColor(LabelDefaultValues::textColor),
	_outlineColor(LabelDefaultValues::outlineColor),
	_font(jgl::Application::instance()->defaultFont()),
	_text("Ceci est un test")
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
	_font.value()->draw(_text, _anchor, 100, _textColor, 2, _outlineColor, _depth);
}

class Test : public jgl::WidgetCore
{
private:
	jgl::Box _box;
	Label _label;

	jgl::Bool _onUpdate()
	{
		return (false);
	}
	void _onRender()
	{
		_box.render();
		_label.render();
	}
	void _onGeometryChange()
	{
		_box.setGeometry(anchor(), size());
		_box.setDepth(depth());

		_label.setGeometry(_box.usableAnchor() + 5, _box.usableSize() - 10);
		_label.setDepth(depth() + 0.5f);
	}

public:
	Test(std::string p_name) : jgl::WidgetCore(p_name)
	{

	}
};

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));

	Test* tmp = app.addRootWidget<Test>("Frame");
	tmp->setGeometry(0, app.size());
	tmp->activate();

	return (app.run());
}