#include "jgl2.h"

class Test : public jgl::Widget
{
private:
	jgl::WidgetComponent::Box _boxes[5];
	jgl::WidgetComponent::TextLabel _labels[5];

	jgl::Bool _onUpdate()
	{
		return (false);
	}
	void _onGeometryChange()
	{
		jgl::Vector2Int boxSize = size() / jgl::Vector2Int(1, 5);
		for (jgl::Size_t i = 0; i < 5; i++)
		{
			_boxes[i].setGeometry(jgl::Vector2Int(0, boxSize.y() * i), boxSize);
			_labels[i].setGeometry(_boxes[i].usableAnchor(), _boxes[i].usableSize());
		}
	}
	void _onRender()
	{
		for (jgl::Size_t i = 0; i < 5; i++)
		{
			_boxes[i].render(depth());
			_labels[i].render(depth() + 0.2f);
		}
	}

public: 
	Test(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		for (jgl::Size_t i = 0; i < 5; i++)
		{
			_boxes[i].setBorderSize(jgl::Vector2Int(5, 5));
			_labels[i].setColor(jgl::Color(255, 255, 255), jgl::Color(0, 0, 0));
			_labels[i].setTextOutlineSize(1);
		}
		_labels[0].setText("abcdefghijkl");
		_labels[1].setText("mnopqrstuv");
		_labels[2].setText("wxyz0123456789");
		_labels[4].setText("~#{[|`\\^@]}");
	}
};

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("JGLTester", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("karma suture.ttf"));

	Test test = Test(nullptr);
	test.setGeometry(jgl::Vector2Int(0, 0), app.size());
	test.activate();

	return (app.run());
}