#include "jgl2.h"

class Box
{
private:
	jgl::Color _colorFrontground;
	jgl::Color _colorBackground;
	jgl::Vector2Int _anchor;
	jgl::Vector2Int _size;
	jgl::Vector2Int _borderSize;

public:
	Box()
	{
		_borderSize = 5;
		_colorFrontground = jgl::Color(255, 0, 0);
		_colorBackground = jgl::Color(255, 255, 0);
	}
	void setBorderSize(jgl::Vector2Int p_borderSize)
	{
		_borderSize = p_borderSize;
	}
	void setColors(jgl::Color p_colorFrontground, jgl::Color p_colorBackground)
	{
		_colorFrontground = p_colorFrontground;
		_colorBackground = p_colorBackground;

	}
	void setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size)
	{
		_anchor = p_anchor;
		_size = p_size;
	}
	void render(jgl::Float p_depth)
	{
		jgl::drawRectangleColor(_colorBackground, _anchor + _borderSize, _size - _borderSize * 2, p_depth);
		jgl::drawRectangleColor(_colorFrontground, _anchor, _size, p_depth);
	}
};

class Test : public jgl::WidgetCore
{
private:
	Box _box;

	jgl::Bool _onUpdate()
	{
		return (false);
	}
	void _onRender()
	{
		_box.render(depth());
	}
	void _onGeometryChange()
	{
		_box.setGeometry(anchor(), size());
	}

public:
	Test(std::string p_name) : jgl::WidgetCore(p_name)
	{
		viewport()->setOffset(5, 10);
	}
	Box& box() { return (_box); }
};

std::vector<Test*> add4corners(jgl::WidgetCore* p_parent, jgl::Vector2Int p_size)
{
	std::vector<Test*> result;
	result.resize(4);
	for (jgl::Size_t i = 0; i < 4; i++)
	{
		result[i] = p_parent->addChildren<Test>("Corner[" + std::to_string(i) + "]");
		result[i]->box().setColors(jgl::Color(66, 138, 245), jgl::Color(47, 101, 181));
		result[i]->setGeometry(jgl::Vector2Int(i % 2, i / 2) * p_parent->usableSize() - p_size / 2, p_size);
		result[i]->activate();
	}
	return (result);
}
std::vector<Test*> add4subCorners(jgl::WidgetCore* p_parent, jgl::Vector2Int p_size)
{
	std::vector<Test*> result;
	result.resize(4);
	for (jgl::Size_t i = 0; i < 4; i++)
	{
		result[i] = p_parent->addChildren<Test>("Corner[" + std::to_string(i) + "]");
		result[i]->box().setColors(jgl::Color(66, 138, 245), jgl::Color(47, 101, 181));
		result[i]->setGeometry(0, p_size);
		result[i]->activate();
	}
	return (result);
}

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));

	Test* tmp = app.addRootWidget<Test>("Test");
	tmp->box().setColors(jgl::Color(67, 222, 109), jgl::Color(50, 168, 82));
	tmp->setGeometry(250, 300);
	tmp->activate();

	std::vector<Test*> cornersA = add4corners(tmp, 150);

	add4subCorners(cornersA[0], cornersA[0]->usableSize() / 2);
	add4subCorners(cornersA[1], cornersA[1]->usableSize() / 2);
	add4subCorners(cornersA[2], cornersA[2]->usableSize() / 2);
	add4subCorners(cornersA[3], cornersA[3]->usableSize() / 2);

	return (app.run());
}