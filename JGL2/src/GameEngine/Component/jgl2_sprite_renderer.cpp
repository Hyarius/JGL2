#include "GameEngine/Component/jgl2_sprite_renderer.h"
#include "GameEngine/jgl2_game_object.h"
#include "GameEngine/jgl2_scene.h"

void SpriteRenderer::_initializeStaticOpenGL()
{
	if (_shader == nullptr)
	{
		if (jgl::Application::instance()->shader("SpriteRendererShader") == nullptr)
			jgl::Application::instance()->addShader("SpriteRendererShader", new jgl::Shader(C_VERTEX_SHADER_CODE, C_FRAGMENT_SHADER_CODE));
		_shader = jgl::Application::instance()->shader("SpriteRendererShader");
	}

	if (_matrixUniform == nullptr)
		_matrixUniform = _shader->uniform("MVP")->copy();
	if (_textureUniform == nullptr)
		_textureUniform = _shader->uniform("textureID")->copy();

	_staticInitialized = true;
}

void SpriteRenderer::_initializeOpenGL()
{
	if (_staticInitialized == false)
		_initializeStaticOpenGL();

	_modelVertexBuffer = _shader->buffer("model_space")->copy();
	_modelUVSBuffer = _shader->buffer("model_uvs")->copy();
	_elementBuffer = _shader->elementBuffer()->copy();

	_initialized = true;
}

void SpriteRenderer::_bake()
{
	if (_initialized == false)
		_initializeOpenGL();

	_modelVertexBuffer->send(_mesh.vertices.data(), static_cast<jgl::Size_t>(_mesh.vertices.size()));
	_modelUVSBuffer->send(_mesh.uvs.data(), static_cast<jgl::Size_t>(_mesh.uvs.size()));
	_elementBuffer->send(_mesh.elements.data(), static_cast<jgl::Size_t>(_mesh.elements.size()));

	_baked = true;
}

void SpriteRenderer::_onRender()
{
	if (_mesh.texture == nullptr)
		throw std::runtime_error("No texture set to render a GameObject2D");

	if (_baked == false)
		_bake();

	_shader->activate();

	_mesh.texture->activate();

	_matrixUniform->send(Scene::activeScene->mainCamera()->mvp());
	_textureUniform->send(0);
	_modelVertexBuffer->activate();
	_modelUVSBuffer->activate();
	_elementBuffer->activate();

	_shader->cast(jgl::Shader::Mode::Triangle, _elementBuffer->size() / sizeof(jgl::UInt));
}

void SpriteRenderer::_onUpdate()
{

}

SpriteRenderer::SpriteRenderer(GameObject* p_owner) : Component(p_owner)
{
	unbake();
}

void SpriteRenderer::setMesh(const Mesh& p_mesh)
{
	_mesh = p_mesh;
	unbake();
}

void SpriteRenderer::unbake()
{
	_baked = false;
}
