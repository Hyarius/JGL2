#include "jgl2.h"

class TestWidget : public jgl::Abstract::Widget
{
private:
	jgl::Image* _image;

	jgl::Bool _onUpdate()
	{
		return (false);
	}

	void _onRender()
	{
		_image->draw(jgl::Vector2Int(100, 100), jgl::Vector2Int(200, 200), jgl::Vector2(0, 0), jgl::Vector2(1, 1));
	}

public:
	TestWidget(jgl::Abstract::Widget* p_parent) : jgl::Abstract::Widget(p_parent)
	{
		_image = new jgl::Image("test.png");
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