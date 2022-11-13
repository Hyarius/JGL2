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
		_box.setGeometry(anchor(), size());
		_label.setGeometry(_box.usableAnchor(), _box.usableSize());
	}
	void _onRender()
	{
		_box.render();
		_label.render();
	}

public:
	Test(jgl::Widget* p_parent) : jgl::Widget(p_parent),
		_box(this),
		_label(this)
	{
		_setViewportOffset(jgl::Vector2Int(5, 5), jgl::Vector2Int(10, 10));
		_box.setBorderSize(jgl::Vector2Int(5, 5));
	}
};

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("JGLTester", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	jgl::cout.setEncoding("fr-FR");

	Test test = Test(nullptr);
	test.setGeometry(jgl::Vector2Int(10, 10), jgl::Vector2Int(300, 60));
	test.activate();

	return (app.run());
}