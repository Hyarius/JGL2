#include "jgl2.h"

class TestWidget : public jgl::Abstract::Widget
{
private:

	jgl::Bool _onUpdate()
	{
		return (false);
	}

	void _onRender()
	{
		jgl::drawRectangleColor(jgl::Color(255, 0, 0), jgl::Vector2Int(100, 100), jgl::Vector2Int(100, 100));
		jgl::drawRectangleColor(jgl::Color(0, 255, 0), jgl::Vector2Int(200, 200), jgl::Vector2Int(100, 100));
	}

public:
	TestWidget(jgl::Abstract::Widget* p_parent) : jgl::Abstract::Widget(p_parent)
	{

	}
};

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("JGLTester", jgl::Vector2Int(400, 400));
	jgl::cout.setEncoding("fr-FR");

	TestWidget tmpWidget = TestWidget(nullptr);
	tmpWidget.activate();

	return (app.run());
}