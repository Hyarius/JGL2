#include "jgl2.h"

class Frame : public jgl::Widget
{
private:
	jgl::Color _backgroundColor = jgl::Color(120, 120, 120);

	jgl::Bool _onUpdate()
	{
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::D) == jgl::InputStatus::Released)
		{
			move(jgl::Vector2Int(10, 0));
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Q) == jgl::InputStatus::Released)
		{
			move(jgl::Vector2Int(-10, 0));
		}
		return (false);
	}

	void _onRender()
	{
		jgl::drawRectangleColor(_backgroundColor, 0, size());
	}

	void _onGeometryChange()
	{
		
	}

public:
	Frame(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{

	}
	void setColor(jgl::Color p_color)
	{
		_backgroundColor = p_color;
	}
};

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("JGLTester", jgl::Vector2Int(400, 400), jgl::Color(50, 50, 50));
	jgl::cout.setEncoding("fr-FR");

	Frame* tmpWidget = new Frame(nullptr);
	tmpWidget->setName("Main widget");
	tmpWidget->setGeometry(0, 200);
	tmpWidget->setColor(jgl::Color(255, 0, 0));
	tmpWidget->activate();

	Frame* tmpWidget2 = new Frame(tmpWidget);
	tmpWidget2->setName("Widget children");
	tmpWidget2->setGeometry(10, 180);
	tmpWidget2->setColor(jgl::Color(0, 255, 0));
	tmpWidget2->activate();
	
	Frame* tmpWidget3 = new Frame(tmpWidget2);
	tmpWidget3->setName("Widget children's children");
	tmpWidget3->setGeometry(10, 260);
	tmpWidget3->setColor(jgl::Color(0, 0, 0, 120));
	tmpWidget3->activate();

	return (app.run());
}