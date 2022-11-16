#include "jgl2.h"

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("JGLTester", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("karma suture.ttf"));

	jgl::Button test = jgl::Button([&]() {
		jgl::cout << "Clicked !" << jgl::endl;
		}, nullptr);
	test.setGeometry(jgl::Vector2Int(0, 0), app.size() / jgl::Vector2Int(1, 5));
	test.activate();

	return (app.run());
}