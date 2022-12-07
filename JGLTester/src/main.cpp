#include "jgl2.h"

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("ProjectName", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("karma suture.ttf"));
	app.activateMultiThread();

	jgl::Frame* background = new jgl::Frame(nullptr);
	background->setGeometry(jgl::Vector2Int(0, 0), app.size());
	background->activate();

	jgl::TextLabel* background2 = new jgl::TextLabel("TextLabel1", background);
	background2->setGeometry(jgl::Vector2Int(100, 100), jgl::Vector2Int(300, 60));
	background2->activate();

	jgl::TextLabel* background3 = new jgl::TextLabel("TextLabel2", background);
	background3->setGeometry(jgl::Vector2Int(10, 10), jgl::Vector2Int(200, 60));
	background3->activate();

	jgl::TextLabel* background4 = new jgl::TextLabel("TextLabel3", background);
	background4->setGeometry(jgl::Vector2Int(215, 215), jgl::Vector2Int(200, 60));
	background4->activate();

	app.run();

	jgl::cout << "RenderFPS : " << app.nbRenderFrame() << jgl::endl;
	jgl::cout << "UpdateFPS : " << app.nbUpdateFrame() << jgl::endl;

	return (0);
}