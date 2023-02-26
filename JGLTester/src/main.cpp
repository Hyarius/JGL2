#include "jgl2.h"

int main(int argc, char** argv)
{
	jgl::Application::Graphical app = jgl::Application::Graphical("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));

	jgl::Widget::DebugScreen<4, 20>* tmp = app.addRootWidget<jgl::Widget::DebugScreen<4, 20>>("Frame");
	for (jgl::Size_t i = 0; i < 4; i++)
	{
		for (jgl::Size_t j = 0; j < 20; j++)
		{
			tmp->setText(std::string(jgl::generateNumber(1, 15), 'c'), i, j);
		}
	}
	tmp->setGeometry(0, app.size());
	tmp->activate();

	return (app.run());
}