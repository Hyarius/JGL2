#include "Structure/jgl2_image.h"
#include "Structure/jgl2_application.h"

#define STB_IMAGE_IMPLEMENTATION
#include "ExternalLibraries/stb_image.h"

namespace jgl
{
	Shader* Image::_shader = nullptr;

	Buffer* Image::_model_space_buffer = nullptr;
	Buffer* Image::_vertexUV_buffer = nullptr;
	Buffer* Image::_alpha_value_buffer = nullptr;
	Buffer* Image::_element_index_buffer = nullptr;
	Uniform* Image::_texture_uniform = nullptr;

	void Image::_draw(
		Vector2Int pos_a, Vector2Int pos_b, Vector2Int pos_c,
		Vector2 uv_a, Vector2 uv_b, Vector2 uv_c,
		Float depth, Float alpha)
	{
		if (_id == -1 || Application::instance() == nullptr)
			throw std::runtime_error("Trying to print an image non-initialized or with no application created");

		activate();

		const std::string shader_name = "Texture_shader_2D";

		static UInt element_index[3] = { 0, 3, 1 };

		Vector3 vertex_content[3] = {
			Vector3(Application::instance()->convertScreenToOpenGL(pos_a), depth),
			Vector3(Application::instance()->convertScreenToOpenGL(pos_b), depth),
			Vector3(Application::instance()->convertScreenToOpenGL(pos_c), depth)
		};
		Vector2 uv_content[3] = {
			uv_a, uv_b, uv_c
		};
		Float alpha_content[3] = {
			alpha, alpha, alpha
		};

		if (_shader == nullptr)
			_shader = Application::instance()->shader(shader_name);

		if (_shader == nullptr)
		{
			std::string errorMessage = "No shader named " + shader_name;
			throw std::runtime_error(errorMessage.c_str());

		}

		if (_model_space_buffer == nullptr)
			_model_space_buffer = _shader->buffer("model_space");

		if (_vertexUV_buffer == nullptr)
			_vertexUV_buffer = _shader->buffer("vertexUV");

		if (_alpha_value_buffer == nullptr)
			_alpha_value_buffer = _shader->buffer("alpha_value");

		if (_element_index_buffer == nullptr)
			_element_index_buffer = _shader->elementBuffer();

		if (_texture_uniform == nullptr)
			_texture_uniform = _shader->uniform("textureID");

		_model_space_buffer->send(vertex_content, 3);
		_vertexUV_buffer->send(uv_content, 3);
		_alpha_value_buffer->send(alpha_content, 3);
		_element_index_buffer->send(element_index, 3);
		_texture_uniform->send(0);

		_shader->launch(Shader::Mode::Triangle);

		desactivate();
	}

	Image::Image(GLuint p_id) : _data(nullptr)
	{
		_id = p_id;
	}

	Image::Image(std::string path) : _data(nullptr)
	{
		_data = stbi_load(path.c_str(), &_width, &_height, &_nbChannels, 0);

		if (_data == nullptr)
		{
			std::string errorMessage = "Can't load file " + path;
			throw std::runtime_error(errorMessage.c_str());
		}

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		if (_nbChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
		else if (_nbChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
		if (Application::instance() == nullptr)
			throw std::runtime_error("No application started while loading an image");

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	Image::~Image()
	{
		if (_id != -1)
			glDeleteTextures(1, &_id);
	}

	UInt Image::pixel(UInt p_x, UInt p_y)
	{
		if (p_x > static_cast<UInt>(_width) || p_y > static_cast<UInt>(_height))
		{
			throw std::runtime_error("Pixel out of range");
		}
		unsigned char datas[4] = { 0, 0, 0, 0 };

		UInt index = (p_x * _nbChannels) + (_width * _nbChannels) * p_y;
		for (Size_t comp = 0; comp < _nbChannels; comp++)
			datas[comp] = _data[index + (_nbChannels - 1 - comp)];

		if (_nbChannels == 3)
		{
			datas[3] = 0xFF;
		}

		return (*(reinterpret_cast<UInt*>(datas)));
	}

	void Image::activate(GLenum texture)
	{
		glActiveTexture(texture);
		glBindTexture(GL_TEXTURE_2D, _id);
	}

	void Image::desactivate(GLenum texture)
	{
		glActiveTexture(texture);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}