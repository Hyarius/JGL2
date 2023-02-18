#include "jgl2.h"

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));

	jgl::Frame* tmp = app.addRootWidget<jgl::Frame>("Frame");
	tmp->box().setColors(jgl::Color(67, 222, 109), jgl::Color(50, 168, 82));
	tmp->setGeometry(300, 300);
	tmp->activate();

	jgl::Frame* tmp2 = app.addRootWidget<jgl::Frame>("Frame2");
	tmp2->box().setColors(jgl::Color(67, 222, 109), jgl::Color(50, 168, 82));
	tmp2->setGeometry(100, 100);
	tmp2->activate();

	return (app.run());
}