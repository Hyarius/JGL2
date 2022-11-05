#include "jgl2.h"

class TestWidget : public jgl::Abstract::Widget
{
private:
	jgl::Font* _font;

	jgl::Bool _onUpdate()
	{
		return (false);
	}

	void _onRender()
	{
		jgl::Size_t text_size = 160;
		_font->draw("Coucou", jgl::Vector2Int(100, 100), text_size, jgl::Color(255, 255, 255));
	}

public:
	TestWidget(jgl::Abstract::Widget* p_parent) : jgl::Abstract::Widget(p_parent)
	{
		_font = new jgl::Font("karma suture.ttf", 80, 1);
	}
};

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("JGLTester", jgl::Vector2Int(400, 400), jgl::Color(150, 150, 150));
	jgl::cout.setEncoding("fr-FR");

	TestWidget tmpWidget = TestWidget(nullptr);
	tmpWidget.activate();

	return (app.run());
}