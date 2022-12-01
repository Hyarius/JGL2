#include "jgl2.h"

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("ProjectName", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	app.activateMultiThread();

	return (app.run());
}