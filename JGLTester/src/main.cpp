#include "jgl2.h"

class Test : public jgl::Widget
{
private:
	jgl::WidgetComponent::Box _box;
	jgl::WidgetComponent::TextLabel _label;

	jgl::Bool _onUpdate()
	{
		return (false);
	}
	void _onGeometryChange()
	{
		_box.setGeometry(jgl::Vector2Int(0, 0), size());
		_label.setGeometry(_box.usableAnchor(), _box.usableSize());
	}
	void _onRender()
	{
		_box.render(depth());
		_label.render(depth() + 0.2f);
	}

public:
	Test(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_setViewportOffset(jgl::Vector2Int(5, 5), jgl::Vector2Int(10, 10));
		_box.setBorderSize(jgl::Vector2Int(5, 5));
		_label.setColor(jgl::Color(255, 255, 255), jgl::Color(0, 0, 0));
		_label.setText("Ceci est un test");
	}
};

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("JGLTester", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("karma suture.ttf", 90, 1));

	Test test = Test(nullptr);
	test.setGeometry(jgl::Vector2Int(10, 10), jgl::Vector2Int(300, 60));
	test.activate();

	return (app.run());
}