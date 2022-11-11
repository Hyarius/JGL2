#include "jgl2.h"

class Test : public jgl::Widget
{
private:
	jgl::Frame* _firstFrame;
	jgl::Frame* _secondFrame;
	jgl::Frame* _selectedFrame;

	jgl::Bool _onUpdate()
	{
		static jgl::ULong nextInput = 0;
		jgl::ULong inputDelay = 150;

		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Key_1) == jgl::InputStatus::Released)
		{
			_selectedFrame = _firstFrame;
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Key_2) == jgl::InputStatus::Released)
		{
			_selectedFrame = _secondFrame;
		}

		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Z) == jgl::InputStatus::Down && jgl::Application::instance()->time() > nextInput)
		{
			_selectedFrame->move(jgl::Vector2Int(0, -10));
			nextInput = jgl::Application::instance()->time() + inputDelay;
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::S) == jgl::InputStatus::Down && jgl::Application::instance()->time() > nextInput)
		{
			_selectedFrame->move(jgl::Vector2Int(0, 10));
			nextInput = jgl::Application::instance()->time() + inputDelay;
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Q) == jgl::InputStatus::Down && jgl::Application::instance()->time() > nextInput)
		{
			_selectedFrame->move(jgl::Vector2Int(-10, 0));
			nextInput = jgl::Application::instance()->time() + inputDelay;
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::D) == jgl::InputStatus::Down && jgl::Application::instance()->time() > nextInput)
		{
			_selectedFrame->move(jgl::Vector2Int(10, 0));
			nextInput = jgl::Application::instance()->time() + inputDelay;
		}

		return (false);
	}

	void _onGeometryChange()
	{
		jgl::Vector2Int tmp_anchor = jgl::Vector2Int(50, 50);
		jgl::Vector2Int tmp_size = size() - tmp_anchor * jgl::Vector2Int(2, 2);

		_firstFrame->setGeometry(tmp_anchor - jgl::Vector2Int(5, 5), tmp_size);

		_secondFrame->setGeometry(jgl::Vector2Int(-10, 300), jgl::Vector2Int(400, 400));

		_selectedFrame = _secondFrame;
	}

	void _onRender()
	{
		jgl::drawRectangleColor(jgl::Color(255, 255, 255), jgl::Vector2Int(750, 750), jgl::Vector2Int(10, 10), 150);
	}

public:
	Test(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_firstFrame = new jgl::Frame(this);
		_firstFrame->setName("firstName");
		_firstFrame->setColor(jgl::Color(255, 0, 0), jgl::Color(255, 100, 100));
		_firstFrame->activate();

		_secondFrame = new jgl::Frame(_firstFrame);
		_secondFrame->setName("secondName");
		_secondFrame->setColor(jgl::Color(0, 255, 0), jgl::Color(20, 180, 100));
		_secondFrame->activate();
	}
};

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("JGLTester", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	jgl::cout.setEncoding("fr-FR");

	Test* test = new Test(nullptr);
	test->setGeometry(jgl::Vector2Int(0, 0), app.size());
	test->activate();

	return (app.run());
}