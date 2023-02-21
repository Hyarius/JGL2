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
		_label.setTextSize(_label.size().y);
	}

public:
	Test2(std::string p_name) : jgl::WidgetCore(p_name)
	{
		_label.setText("Ceci est un test");
		_label.setOutlineSize(100);
	}
};


class Test : public jgl::WidgetCore
{
private:
	std::vector<jgl::InputController> movementController;
	jgl::Size_t _imageIndex = 1;
	jgl::Image* _image[2] = {nullptr, nullptr};
	jgl::Vector2 _position = 0;
	jgl::Float _zoom = 1;

	jgl::Bool _onUpdate()
	{
		for (jgl::Size_t i = 0; i < movementController.size(); i++)
		{
			movementController[i].update();
		}
		if (jgl::Application::instance()->keyboard()->getKey(jgl::Keyboard::F) == jgl::InputStatus::Released)
		{
			_imageIndex = (_imageIndex == 0 ? 1 : 0);
		}
		return (false);
	}
	void _onRender()
	{
		if (_image[_imageIndex] != nullptr)
		{
			_image[_imageIndex]->draw(_position, jgl::Vector2(size().x, size().y) * _zoom, 0, 1, 2);
		}
	}
	void _onGeometryChange()
	{

	}

	jgl::Shader* shader;

	jgl::Uniform* textureUniform;
	jgl::Uniform* coefFlouUniform;
	jgl::Uniform* coefOriginalUniform;
	jgl::Buffer* verticesBuffer;
	jgl::Buffer* uvsBuffer;
	jgl::Buffer* elementBuffer;

	void initShaderData()
	{
		jgl::ImageOutput output = jgl::ImageOutput(jgl::Vector2Int(300, 300));

		output.associate();

		jgl::drawRectangleColor(jgl::Color::green, 100, 100, 20);
		jgl::drawRectangleColor(jgl::Color::red, 10, 100, 10);

		_image[1] = output.save();

		output.desassociate();
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
		_image[0] = new jgl::Image(jgl::Application::instance()->defaultFont()->textureID(100, 10));

		initShaderData();
	}
};

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));

	try
	{
	app.shaders().add("GaussShader", new jgl::Shader(R"(
			#version 330 core

			layout(location = 0) in vec3 vertexPosition;
			layout(location = 1) in vec2 vertexUV;

			out vec2 uv;

			void main()
			{
				gl_Position = vec4(vertexPosition, 1.0);
				uv = vertexUV;
			})", R"(
			#version 330 core

in vec2 uv;
out vec4 fragColor;

uniform sampler2D texture;
uniform float coef_flou;
uniform float coef_original;

const int kernelSize = 7;
const float kernel[49] = float[](0.000308f, 0.004362f, 0.026369f, 0.081358f, 0.135859f, 0.081358f, 0.026369f,
                                0.004362f, 0.061356f, 0.370549f, 1.138394f, 1.906854f, 1.138394f, 0.370549f,
                                0.026369f, 0.370549f, 2.240843f, 6.889984f, 11.571054f, 6.889984f, 2.240843f,
                                0.081358f, 1.138394f, 6.889984f, 21.163803f, 35.538743f, 21.163803f, 6.889984f,
                                0.135859f, 1.906854f, 11.571054f, 35.538743f, 59.517268f, 35.538743f, 11.571054f,
                                0.081358f, 1.138394f, 6.889984f, 21.163803f, 35.538743f, 21.163803f, 6.889984f,
                                0.026369f, 0.370549f, 2.240843f, 6.889984f, 11.571054f, 6.889984f, 2.240843f);

void main()
{
    // Pass 1 : flou gaussien
    vec4 sum = vec4(0.0);
    vec2 texelSize = vec2(1.0) / textureSize(texture, 0);
    
    for(int x = -3; x <= 3; x++) {
        for(int y = -3; y <= 3; y++) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            sum += texture2D(texture, uv + offset) * kernel[(x+3)*kernelSize + (y+3)];
        }
    }
    
    // Pass 2 : combinaison de l'image originale avec l'image floutée
    vec4 originalColor = texture2D(texture, uv);
    vec4 finalColor = originalColor * coef_original + sum * coef_flou;
    
    fragColor = finalColor;
}

			)"));

	}
	catch (std::exception& e)
	{
		jgl::cout << e.what() << std::endl;
		return (0);
	}


	Test* tmp = app.addRootWidget<Test>("Frame");
	tmp->setGeometry(0, app.size());
	//tmp->setGeometry(100, jgl::Vector2Int(300, 100));
	tmp->activate();

	return (app.run());
}