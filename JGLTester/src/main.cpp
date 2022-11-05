#include "jgl2.h"

class TestWidget : public jgl::Abstract::Widget
{
private:
	jgl::SpriteSheet* _image;

	jgl::Bool _onUpdate()
	{
		return (false);
	}

	void _onRender()
	{
		_image->draw(0, jgl::Vector2Int(100, 100), jgl::Vector2Int(100, 100));
		_image->draw(1, jgl::Vector2Int(200, 100), jgl::Vector2Int(100, 100));
		_image->draw(2, jgl::Vector2Int(100, 200), jgl::Vector2Int(100, 100));
		_image->draw(3, jgl::Vector2Int(200, 200), jgl::Vector2Int(100, 100));
		jgl::drawRectangleColor(jgl::Color(255, 0, 0), jgl::Vector2Int(0, 0), jgl::Vector2Int(100, 100));
	}

public:
	TestWidget(jgl::Abstract::Widget* p_parent) : jgl::Abstract::Widget(p_parent)
	{
		_image = new jgl::SpriteSheet("test.png", jgl::Vector2Int(2, 2));
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