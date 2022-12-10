#include "Structure/jgl2_game_object_2D.h"

namespace jgl
{
	void GameObject2D::_initOpenGLData()
	{
		if (_initializedStatic == false)
			_initStaticOpenGLData();

		_modelVertexBuffer = _shader->buffer("model_space")->copy();
		_modelUVSBuffer = _shader->buffer("model_uvs")->copy();
		_elementBuffer = _shader->elementBuffer()->copy();

		_initialized = true;
	}

	void GameObject2D::_initStaticOpenGLData()
	{
		if (_shader == nullptr)
			_shader = jgl::Application::instance()->shader("Orthographic2DShader");

		if (_matrixUniform == nullptr)
			_matrixUniform = _shader->uniform("MVP")->copy();
		if (_depthUniform == nullptr)
			_depthUniform = _shader->uniform("depth")->copy();
		if (_textureUniform == nullptr)
			_textureUniform = _shader->uniform("textureID")->copy();

		_initializedStatic = true;
	}

	void GameObject2D::bake()
	{
		if (_initialized == false)
			_initOpenGLData();

		_vertexDataArray.clear();
		_uvsDataArray.clear();
		_elementDataArray.clear();

		_onBake(_vertexDataArray, _uvsDataArray, _elementDataArray);

		_modelVertexBuffer->send(_vertexDataArray.data(), static_cast<jgl::Size_t>(_vertexDataArray.size()));
		_modelUVSBuffer->send(_uvsDataArray.data(), static_cast<jgl::Size_t>(_uvsDataArray.size()));
		_elementBuffer->send(_elementDataArray.data(), static_cast<jgl::Size_t>(_elementDataArray.size()));

		_baked = true;
	}

	void GameObject2D::sendMVPMatrix(glm::mat4 p_MVP)
	{
		if (_initializedStatic == false)
			_initStaticOpenGLData();

		_shader->activate();

		_matrixUniform->send(p_MVP);
	}
	void GameObject2D::setTexture(jgl::Image* p_texture)
	{
		_texture = p_texture;
	}
	void GameObject2D::render(jgl::Float p_depth)
	{
		if (_baked == false)
			bake();

		if (_texture == nullptr)
			throw std::runtime_error("No texture set to render a GameObject2D");

		_shader->activate();

		_texture->activate();

		_depthUniform->send(p_depth);
		_textureUniform->send(0);
		_modelVertexBuffer->activate();
		_modelUVSBuffer->activate();
		_elementBuffer->activate();

		_shader->cast(jgl::Shader::Mode::Triangle, _elementBuffer->size() / sizeof(jgl::UInt));
	}
}