#include "jgl2.h"

int main(int argc, char** argv)
{
	jgl::Application::Graphical app = jgl::Application::Graphical("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));

	jgl::Widget::Button* tmp = app.addRootWidget<jgl::Widget::Button>("Frame", [&]() {jgl::cout << "Coucou !" << std::endl; });
	tmp->unselectedLabel().setText("Unselected");
	tmp->selectedLabel().setText("Selected");
	tmp->setGeometry(app.size() / 4, app.size() / 2);
	tmp->activate();

	return (app.run());
}