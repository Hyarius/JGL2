#include "jgl2.h"

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("JGLTester", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	jgl::cout.setEncoding("fr-FR");

	jgl::Frame* tmp = new jgl::Frame(nullptr);
	tmp->setGeometry(jgl::Vector2Int(0, 0), jgl::Vector2Int(200, 200));
	tmp->activate();

	jgl::Frame* tmp2 = new jgl::Frame(tmp);
	tmp2->setGeometry(jgl::Vector2Int(5, 5), jgl::Vector2Int(100, 100));
	tmp2->activate();

	return (app.run());
}