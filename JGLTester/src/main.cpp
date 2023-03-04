#include "jgl2.h"

int main(int argc, char** argv)
{
	jgl::Application::Graphical app = jgl::Application::Graphical("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));

	jgl::Widget::Entry* textEntry = app.addRootWidget<jgl::Widget::Entry>("TestEntry");
	textEntry->setPlaceholder("My custom place holder");
	textEntry->setGeometry(300, jgl::Vector2Int(300, 70));
	textEntry->activate();

	return (app.run());
}