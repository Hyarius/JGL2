#include "jgl2.h"

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::GraphicalApplication app = jgl::GraphicalApplication("CardGame", jgl::Vector2Int(900, 600), jgl::Color(60, 60, 60));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));
	app.setMaxDepth(1000);

	jgl::TextLabel* mainApp = new jgl::TextLabel("peut-etre", nullptr);
	mainApp->label().setVerticalAlignment(jgl::VerticalAlignment::Centred);
	mainApp->label().setHorizontalAlignment(jgl::HorizontalAlignment::Centred);
	//mainApp->setDefinedTextSize(30);
	mainApp->setDepth(100);
	mainApp->setGeometry(jgl::Vector2Int(300, 200), app.size() - jgl::Vector2Int(600, 400));
	mainApp->activate();

	return (app.run());
}