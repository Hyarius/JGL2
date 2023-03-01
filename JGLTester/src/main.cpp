#include "jgl2.h"

int main(int argc, char** argv)
{
	jgl::Application::Graphical app = jgl::Application::Graphical("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));

	return (app.run());
}