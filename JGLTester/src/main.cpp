#include "jgl2.h"

namespace jgl
{
	class Frame : public jgl::Widget
	{
	private:
		jgl::Color _backgroundColor = jgl::Color(120, 120, 120);
		jgl::Color _frontgroundColor = jgl::Color(150, 150, 150);

		jgl::Bool _onUpdate()
		{
			return (false);
		}

		void _onRender()
		{
			jgl::drawRectangleColor(_backgroundColor, anchor(), size(), depth());
			jgl::drawRectangleColor(_frontgroundColor, anchor() + jgl::Vector2Int(5, 5), size() - jgl::Vector2Int(10, 10), depth() + 0.1f);
		}

		void _onGeometryChange()
		{

		}

	public:
		Frame(jgl::Widget* p_parent) : jgl::Widget(p_parent)
		{
			_setViewportAnchorOffset(5);
			_setViewportSizeOffset(10);
		}
		void setColor(jgl::Color p_backgroundColor, jgl::Color p_frontgroundColor)
		{
			_frontgroundColor = p_frontgroundColor;
			_backgroundColor = p_backgroundColor;
		}
	};
}

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("JGLTester", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	jgl::cout.setEncoding("fr-FR");

	jgl::Frame* tmpWidget1 = new jgl::Frame(nullptr);
	tmpWidget1->setName("Red frame");
	tmpWidget1->setColor(jgl::Color(255, 0, 0), jgl::Color(255, 100, 100));
	tmpWidget1->setGeometry(40, app.size() - jgl::Vector2Int(80, 80));
	tmpWidget1->activate();

	jgl::Frame* tmpWidget2 = new jgl::Frame(tmpWidget1);
	tmpWidget2->setName("Green frame");
	tmpWidget2->setColor(jgl::Color(0, 255, 0), jgl::Color(100, 255, 100));
	tmpWidget2->setGeometry(0, tmpWidget1->usableSize() - jgl::Vector2Int(80, 80));
	tmpWidget2->activate();

	jgl::Frame* tmpWidget3 = new jgl::Frame(tmpWidget2);
	tmpWidget3->setName("Blue frame");
	tmpWidget3->setColor(jgl::Color(0, 0, 255), jgl::Color(100, 100, 255));
	tmpWidget3->setGeometry(0, tmpWidget2->usableSize() - jgl::Vector2Int(80, 80));
	tmpWidget3->activate();

	jgl::Frame* tmpWidget4 = new jgl::Frame(tmpWidget3);
	tmpWidget4->setName("Magenta frame");
	tmpWidget4->setColor(jgl::Color(255, 0, 255), jgl::Color(255, 100, 255));
	tmpWidget4->setGeometry(0, tmpWidget3->usableSize() - jgl::Vector2Int(80, 80));
	tmpWidget4->activate();

	return (app.run());
}