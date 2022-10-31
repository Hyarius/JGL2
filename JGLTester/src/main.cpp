#include "jgl2.h"

class TestWidget : public jgl::Abstract::Widget
{
private:

	jgl::Bool _onUpdate()
	{
		return (false);
	}

	void _onRender()
	{
		jgl::draw_rectangle_color(jgl::Color(255, 0, 0), jgl::Vector2Int(100, 100), jgl::Vector2Int(100, 100));
	}

public:
	TestWidget(jgl::Abstract::Widget* p_parent) : jgl::Abstract::Widget(p_parent)
	{

	}
};

std::string colorShaderVertex =
R"(#version 330 core
layout(location = 0) in vec3 model_space;
layout(location = 1) in vec4 color_space;
out vec4 fragmentColor;
void main()
{
gl_Position = vec4(model_space, 1.0f);
fragmentColor = color_space;
})";

std::string colorShaderFragment =
R"(#version 330 core
in vec4 fragmentColor;
layout(location = 0) out vec4 color;
void main()
{
	color = fragmentColor;
	if (color.a == 0)
		discard;
})";

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("JGLTester", jgl::Vector2Int(400, 400));
	jgl::cout.setEncoding("fr-FR");

	jgl::Shader tmpShader = jgl::Shader(colorShaderVertex, colorShaderFragment);

	TestWidget tmpWidget = TestWidget(nullptr);

	return (app.run());
}