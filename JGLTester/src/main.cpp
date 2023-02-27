#include "jgl2.h"

class Test : public jgl::Abstract::Widget::Core
{
private:
	jgl::Widget::DebugScreen<4, 20>* _debugScreen;

	jgl::Bool _onUpdate()
	{
		
		return (false);
	}

	void _onGeometryChange()
	{
		_debugScreen->setGeometry(0, size());
	}

	void _onRender()
	{

	}

public:
	Test() : jgl::Abstract::Widget::Core("Test")
	{
		_debugScreen = addChildren<jgl::Widget::DebugScreen<4, 20>>("DebugScreen");
		_debugScreen->activate();

		jgl::Application::Graphical::instance()->addFunctToInvokePerSecond([&]() {
				_debugScreen->setText("Render FPS : " + std::to_string(jgl::Application::Graphical::instance()->nbRenderPerSecond()), 0, 0);
				_debugScreen->setText("Update FPS : " + std::to_string(jgl::Application::Graphical::instance()->nbUpdatePerSecond()), 0, 1);
			});
	}
};

int main(int argc, char** argv)
{
	jgl::Application::Graphical app = jgl::Application::Graphical("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));

	Test* tmp = app.addRootWidget<Test>();
	tmp->setGeometry(0, app.size());
	tmp->activate();

	return (app.run());
}