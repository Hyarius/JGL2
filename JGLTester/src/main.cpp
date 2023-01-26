#include "jgl2.h"

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::GraphicalApplication app = jgl::GraphicalApplication("CardGame", jgl::Vector2Int(900, 600), jgl::Color(60, 60, 60));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));
	app.setMaxDepth(1000);

	jgl::TextLabel* mainApp = new jgl::TextLabel("X", nullptr);
	mainApp->setGeometry(jgl::Vector2Int(100, 100), app.size() - jgl::Vector2Int(200, 200));
	mainApp->activate();

	return (app.run());
}