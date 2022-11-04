#include "Structure/jgl2_image.h"
#include "Structure/jgl2_application.h"

#define STB_IMAGE_IMPLEMENTATION
#include "ExternalLibraries/stb_image.h"

namespace jgl
{
	Shader* Image::_shader = nullptr;

	Buffer* Image::_modelSpaceBuffer = nullptr;
	Buffer* Image::_modelUvBuffer = nullptr;
	Buffer* Image::_indexesBuffer = nullptr;
	Uniform* Image::_textureUniform = nullptr;

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

		Int index = (p_x * _nbChannels) + (_width * _nbChannels) * p_y;
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

	void Image::_init_shader_data()
	{
		if (_shader == nullptr)
			_shader = Application::instance()->shader("Texture2D");

		if (_modelSpaceBuffer == nullptr)
			_modelSpaceBuffer = _shader->buffer("model_space");
		if (_modelUvBuffer == nullptr)
			_modelUvBuffer = _shader->buffer("model_uv");
		if (_indexesBuffer == nullptr)
			_indexesBuffer = _shader->elementBuffer();
		if (_textureUniform == nullptr)
			_textureUniform = _shader->uniform("textureID");

		if (_shader == nullptr)
			throw std::runtime_error("Error : no shader Texture2D in application");

		if (_modelSpaceBuffer == nullptr)
			throw std::runtime_error("Error : no model space buffer found in shader");
		if (_modelUvBuffer == nullptr)
			throw std::runtime_error("Error : no model uv buffer found in shader");
		if (_indexesBuffer == nullptr)
			throw std::runtime_error("Error : no element buffer found in shader");
		if (_textureUniform == nullptr)
			throw std::runtime_error("Error : no texture ID uniform found in shader");
	}

	void Image::draw(Vector2Int pos, Vector2Int size, Vector2 uv_pos, Vector2 uv_size)
	{
		static UInt elementIndex[6] = { 0, 3, 1, 2, 3, 0 };
		static Vector2Int deltaPos[4] = {
			Vector2Int(0, 0),
			Vector2Int(1, 0),
			Vector2Int(0, 1),
			Vector2Int(1, 1)
		};

		_init_shader_data();

		Vector3 vertexContent[4];
		Vector2 uvContent[4];

		for (size_t i = 0; i < 4; i++)
		{
			vertexContent[i] = Vector3(jgl::Application::instance()->convertScreenToOpenGL(pos + size * deltaPos[i]), 0);
			uvContent[i] = (uv_pos + uv_size * deltaPos[i]);
		}

		_modelSpaceBuffer->send(vertexContent, 4);
		_modelUvBuffer->send(uvContent, 4);
		_indexesBuffer->send(elementIndex, 6);
		_textureUniform->send(0);

		_shader->launch(jgl::Shader::Mode::Triangle);
	}
}