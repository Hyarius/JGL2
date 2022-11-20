#include "MainMenu.h"

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("JGLPositioner", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("karma suture.ttf"));

	jgl::Observer<Event>::subscribe(Event::A, []() {jgl::cout << "Event A notified" << jgl::endl; });

	MainMenu* mainMenu = new MainMenu(nullptr);
	mainMenu->setGeometry(jgl::Vector2Int(0, 0), app.size());
	mainMenu->activate();

	return (app.run());
}