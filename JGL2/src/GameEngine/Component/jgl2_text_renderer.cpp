#include "GameEngine/Component/jgl2_text_renderer.h"
#include "GameEngine/jgl2_game_object.h"
#include "GameEngine/jgl2_scene.h"

namespace jgl
{
	void TextRenderer::_initializeStaticOpenGL()
	{
		if (_shader == nullptr)
		{
			if (jgl::Application::instance()->shader("TextRendererShader") == nullptr)
				jgl::Application::instance()->addShader("TextRendererShader", new jgl::Shader(C_VERTEX_SHADER_CODE, C_FRAGMENT_SHADER_CODE));
			_shader = jgl::Application::instance()->shader("TextRendererShader");
		}

		if (_matrixUniform == nullptr)
			_matrixUniform = _shader->uniform("MVP")->copy();
		if (_anchorUniform == nullptr)
			_anchorUniform = _shader->uniform("anchor")->copy();
		if (_textColorUniform == nullptr)
			_textColorUniform = _shader->uniform("textColor")->copy();
		if (_outlineColorUniform == nullptr)
			_outlineColorUniform = _shader->uniform("outlineColor")->copy();
		if (_textureUniform == nullptr)
			_textureUniform = _shader->uniform("textureID")->copy();

		_staticInitialized = true;
	}

	void TextRenderer::_initializeOpenGL()
	{
		if (_staticInitialized == false)
			_initializeStaticOpenGL();

		_modelVertexBuffer = _shader->buffer("model_space")->copy();
		_modelUVSBuffer = _shader->buffer("model_uvs")->copy();
		_elementBuffer = _shader->elementBuffer()->copy();

		_initialized = true;
	}

	jgl::Vector3 TextRenderer::_computeTextOffset()
	{
		jgl::Vector3 result;
		jgl::Vector3 textSize = _mesh.font->calcStringSizeUnit(_mesh.text, _mesh.textWorldHeight);

		switch (_mesh.verticalAlignment)
		{
		case jgl::VerticalAlignment::Top:
		{
			result.y() = 0;
			break;
		}
		case jgl::VerticalAlignment::Centred:
		{
			result.y() = (_mesh.textWorldArea.y() - textSize.y()) / 2;
			break;
		}
		case jgl::VerticalAlignment::Down:
		{
			result.y() = _mesh.textWorldArea.y() - textSize.y();
			break;
		}
		default:
		{
			result.y() = 0;
			break;
		}
		}


		switch (_mesh.horizontalAlignment)
		{
		case jgl::HorizontalAlignment::Left:
		{
			result.x() = 0;
			break;
		}
		case jgl::HorizontalAlignment::Centred:
		{
			result.x() = (_mesh.textWorldArea.x() - textSize.x()) / 2;
			break;
		}
		case jgl::HorizontalAlignment::Right:
		{
			result.x() = _mesh.textWorldArea.x() - textSize.x();
			break;
		}
		default:
		{
			result.x() = 0;
			break;
		}
		}

		return (result);
	}

	void TextRenderer::_bake()
	{
		if (_initialized == false)
			_initializeOpenGL();

		if (_mesh.font == nullptr)
			_mesh.font = jgl::Application::instance()->defaultFont();

		jgl::Vector3 offset = _computeTextOffset();

		_mesh.font->prepareDrawUnit(_mesh.text, offset, _mesh.textWorldHeight, _mesh.oulineSize);

		_mesh.font->exportShaderData(_modelVertexBuffer, _modelUVSBuffer, _elementBuffer);

		_baked = true;
	}

	void TextRenderer::_onRender()
	{
		if (_baked == false)
			_bake();

		_shader->activate();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _mesh.font->textureID(100, _mesh.oulineSize));

		_matrixUniform->send(Scene::activeScene->mainCamera()->mvp());
		_anchorUniform->send(_owner->core().position);
		_textColorUniform->send(_mesh.textColor);
		_outlineColorUniform->send(_mesh.outlineColor);
		_textureUniform->send(0);

		_modelVertexBuffer->activate();
		_modelUVSBuffer->activate();
		_elementBuffer->activate();

		_shader->cast(jgl::Shader::Mode::Triangle, _elementBuffer->size() / sizeof(jgl::UInt));
	}

	void TextRenderer::_onUpdate()
	{

	}

	TextRenderer::TextRenderer(GameObject* p_owner) : Component(p_owner)
	{
		unbake();
	}

	void TextRenderer::setMesh(const TextMesh& p_mesh)
	{
		_mesh = p_mesh;
		unbake();
	}

	void TextRenderer::unbake()
	{
		_baked = false;
	}
}