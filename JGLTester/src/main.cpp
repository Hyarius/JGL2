#include "jgl2.h"

int main(int argc, char** argv)
{
	jgl::Application::Graphical app = jgl::Application::Graphical("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));

	jgl::Widget::Label* tmp = app.addRootWidget<jgl::Widget::Label>("Frame");
	tmp->label().setText("Coucou, ceci est un test");
	tmp->setGeometry(0, app.size());
	tmp->activate();

	return (app.run());
}