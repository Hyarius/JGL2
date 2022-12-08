#include "jgl2.h"

class Test : public jgl::Widget
{
private:
	jgl::Timer _slideTimer = jgl::Timer(500);
	jgl::Vector2Int _startPosition;
	jgl::Vector2Int _endPosition;

	jgl::Frame* background1;
	jgl::Frame* background2;
	jgl::TextLabel* textLabel1;
	jgl::TextLabel* textLabel2;
	jgl::TextLabel* textLabel3;

	jgl::Bool _onUpdate()
	{
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::A) == jgl::InputStatus::Released)
		{
			_startPosition = jgl::Vector2Int(0, 0);
			_endPosition = jgl::Vector2Int(110, 110);
			_slideTimer.start();
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Q) == jgl::InputStatus::Released)
		{
			_startPosition = jgl::Vector2Int(110, 110);
			_endPosition = jgl::Vector2Int(0, 0);
			_slideTimer.start();
		}

		if (anchor() != _endPosition)
		{
			this->place(jgl::Vector2Int::lerp(_startPosition, _endPosition, _slideTimer.percent()));
		}

		return (false);
	}
	void _onRender()
	{

	}
	void _onGeometryChange()
	{
		background1->setGeometry(jgl::Vector2Int(10, 10), size() / jgl::Vector2(1.5f, 1.5f));
		background2->setGeometry(jgl::Vector2Int(10, 10), size() / jgl::Vector2Int(2, 2));
		textLabel1->setGeometry(jgl::Vector2Int(100, 100), jgl::Vector2Int(300, 60));
		textLabel2->setGeometry(jgl::Vector2Int(10, 10), jgl::Vector2Int(200, 60));
		textLabel3->setGeometry(jgl::Vector2Int(215, 215), jgl::Vector2Int(200, 60));

	}

public:
	Test(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		background1 = new jgl::Frame(this);
		background1->activate();

		background2 = new jgl::Frame(background1);
		background2->activate();

		textLabel1 = new jgl::TextLabel("TextLabel1", background2);
		textLabel1->activate();

		textLabel2 = new jgl::TextLabel("TextLabel2", background2);
		textLabel2->activate();

		textLabel3 = new jgl::TextLabel("TextLabel3", background2);
		textLabel3->activate();
	}
};

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("ProjectName", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("karma suture.ttf"));
	app.activateMultiThread();

	Test* background = new Test(nullptr);
	background->setGeometry(jgl::Vector2Int(20, 20), app.size() / jgl::Vector2(1.2f, 1.2f));
	background->activate();

	app.run();

	jgl::cout << "RenderFPS : " << app.nbRenderFrame() << jgl::endl;
	jgl::cout << "UpdateFPS : " << app.nbUpdateFrame() << jgl::endl;

	return (0);
}