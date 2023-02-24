#include "jgl2.h"

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));

	jgl::Label* tmp = app.addRootWidget<jgl::Label>("Frame");
	tmp->label().setText("Coucou, ceci est un test");
	tmp->setGeometry(0, app.size());
	tmp->activate();

	return (app.run());
}