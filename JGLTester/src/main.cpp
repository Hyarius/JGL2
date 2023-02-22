#include "jgl2.h"
class Test2 : public jgl::WidgetCore
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
	Test2(std::string p_name) : jgl::WidgetCore(p_name)
	{
		_label.setText("Ceci est un test");
		_label.setOutlineSize(2);
	}
};


class Test : public jgl::WidgetCore
{
private:
	std::vector<jgl::InputController> movementController;
	jgl::Size_t _imageIndex = 1;
	jgl::Image* _image = nullptr;
	jgl::Vector2 _position = 0;
	jgl::Float _zoom = 1;

	jgl::Bool _onUpdate()
	{
		for (jgl::Size_t i = 0; i < movementController.size(); i++)
		{
			movementController[i].update();
		}
		return (false);
	}
	void _onRender()
	{
		_image->draw(_position, jgl::Vector2(size().x, size().y) * _zoom, 0, 1, 2);
	}
	void _onGeometryChange()
	{

	}

public:
	Test(std::string p_name) : jgl::WidgetCore(p_name),
		movementController({
				jgl::InputController(jgl::Keyboard::A, jgl::InputStatus::Down, [&]() {if (_zoom > 0.5f) _zoom *= 0.9f; if (_zoom < 0.5f) _zoom = 0.5f; }, 50),
				jgl::InputController(jgl::Keyboard::E, jgl::InputStatus::Down, [&]() {_zoom *= 1.1f; }, 50),

				jgl::InputController(jgl::Keyboard::Z, jgl::InputStatus::Down, [&]() {_position.y += _zoom; }, 5),
				jgl::InputController(jgl::Keyboard::Q, jgl::InputStatus::Down, [&]() {_position.x += _zoom; }, 5),
				jgl::InputController(jgl::Keyboard::S, jgl::InputStatus::Down, [&]() {_position.y -= _zoom; }, 5),
				jgl::InputController(jgl::Keyboard::D, jgl::InputStatus::Down, [&]() {_position.x -= _zoom; }, 5)
			})
	{
		_image = new jgl::Image(jgl::Application::instance()->defaultFont()->textureID(100, 10));
	}
};

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));

	Test2* tmp = app.addRootWidget<Test2>("Frame");
	//tmp->setGeometry(0, app.size());
	tmp->setGeometry(100, jgl::Vector2Int(300, 100));
	tmp->activate();

	return (app.run());
}