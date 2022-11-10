#include "jgl2.h"

class Test : public jgl::Widget
{
private:
	jgl::Frame* _firstFrame;
	jgl::Frame* _secondFrame;
	jgl::Frame* _thirdFrame;
	jgl::Frame* _selectedFrame;

	jgl::Bool _onUpdate()
	{
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Key_1) == jgl::InputStatus::Released)
		{
			_selectedFrame = _firstFrame;
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Key_2) == jgl::InputStatus::Released)
		{
			_selectedFrame = _secondFrame;
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Key_3) == jgl::InputStatus::Released)
		{
			_selectedFrame = _thirdFrame;
		}

		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Z) == jgl::InputStatus::Released)
		{
			_selectedFrame->move(jgl::Vector2Int(0, -10));
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::S) == jgl::InputStatus::Released)
		{
			_selectedFrame->move(jgl::Vector2Int(0, 10));
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Q) == jgl::InputStatus::Released)
		{
			_selectedFrame->move(jgl::Vector2Int(-10, 0));
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::D) == jgl::InputStatus::Released)
		{
			_selectedFrame->move(jgl::Vector2Int(10, 0));
		}
		return (false);
	}

	void _onGeometryChange()
	{
		jgl::Vector2Int tmp_anchor = jgl::Vector2Int(50, 50);
		jgl::Vector2Int tmp_size = size() - tmp_anchor * jgl::Vector2Int(2, 2);

		_firstFrame->setGeometry(tmp_anchor - jgl::Vector2Int(5, 5), tmp_size);
		tmp_size -= tmp_anchor * jgl::Vector2Int(2, 2);

		_secondFrame->setGeometry(jgl::Vector2Int(-10, -10), tmp_size);
		tmp_size -= tmp_anchor * jgl::Vector2Int(2, 2);

		_thirdFrame->setGeometry(tmp_anchor + jgl::Vector2Int(50, 50), tmp_size);

		_selectedFrame = _firstFrame;
	}

	void _onRender()
	{

	}

public:
	Test(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_firstFrame = new jgl::Frame(this);
		_firstFrame->setColor(jgl::Color(255, 0, 0), jgl::Color(255, 100, 100));
		_firstFrame->activate();

		_secondFrame = new jgl::Frame(_firstFrame);
		_secondFrame->setColor(jgl::Color(0, 255, 0), jgl::Color(20, 180, 100));
		_secondFrame->activate();

		_thirdFrame = new jgl::Frame(_secondFrame);
		_thirdFrame->setColor(jgl::Color(0, 0, 255), jgl::Color(100, 100, 255));
		_thirdFrame->activate();
	}
};

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("JGLTester", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	app.setMaxDepth(4);
	jgl::cout.setEncoding("fr-FR");

	Test* test = new Test(nullptr);
	test->setGeometry(jgl::Vector2Int(0, 0), app.size());
	test->activate();

	return (app.run());
}