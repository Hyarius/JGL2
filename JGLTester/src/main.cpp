#include "jgl2.h"

class Test : public jgl::WidgetCore
{
private:
	jgl::Box _box;
	jgl::Label _label;

	jgl::Bool _onUpdate()
	{
		return (false);
	}
	void _onRender()
	{
		_label.render();
		_box.render();
	}
	void _onGeometryChange()
	{
		_box.setGeometry(anchor(), size());
		_box.setDepth(depth());

		_label.setGeometry(_box.usableAnchor(), _box.usableSize());
		_label.setDepth(depth() + 0.5f);
		_label.setTextSize(40);
	}

public:
	Test(std::string p_name) : jgl::WidgetCore(p_name)
	{
		_label.setText("Ceci est un test");
		_label.setOutlineSize(8);
	}
};

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));

	Test* tmp = app.addRootWidget<Test>("Frame");
	tmp->setGeometry(0, app.size());
	tmp->activate();

	return (app.run());
}