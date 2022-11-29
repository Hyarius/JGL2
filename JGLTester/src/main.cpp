#include "jgl2.h"

class TestWidget : public jgl::Widget
{
private:
	static const jgl::Int C_DEPTH = 10;
	static const jgl::Int C_SIZE = 200;
	jgl::Perlin perlin;
	jgl::Int randomType = 0;
	jgl::Int content[C_DEPTH][C_SIZE][C_SIZE];

	jgl::Bool _onUpdate()
	{
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::A) == jgl::InputStatus::Released && randomType > 0)
			randomType--;
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Z) == jgl::InputStatus::Released && randomType < C_DEPTH - 1)
			randomType++;
		return (false);
	}

	void _onRender()
	{
		jgl::Vector2Int unit = size() / jgl::Vector2Int(C_SIZE, C_SIZE);

		for (jgl::Size_t i = 0; i < C_SIZE; i++)
		{
			for (jgl::Size_t j = 0; j < C_SIZE; j++)
			{
				jgl::drawRectangleColor(jgl::Color(content[randomType][i][j], content[randomType][i][j], content[randomType][i][j]), unit * jgl::Vector2Int(i, j), unit, depth());
			}
		}
	}

	void _onGeometryChange()
	{

	}

public:
	TestWidget(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		perlin.configureRange(0, 256);
		perlin.configureFrequency(30);
		for (jgl::Int i = 0; i < C_SIZE; i++)
		{
			for (jgl::Int j = 0; j < C_SIZE; j++)
			{
				for (jgl::Size_t h = 0; h < C_DEPTH; h++)
				{
					perlin.setOctaveValue(h + 1);
					content[h][i][j] = perlin.sample2D(i - 15000, j - 15000);
				}
			}
		}
	}
};

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("ProjectName", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	app.activateMultiThread();

	TestWidget* test = new TestWidget(nullptr);
	test->setGeometry(jgl::Vector2Int(0, 0), app.size());
	test->activate();

	return (app.run());
}