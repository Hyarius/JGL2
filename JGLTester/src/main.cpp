#include "jgl2_game_engine.h"

using DebugScreen = jgl::SingletonWidget<jgl::DebugScreen<3, 25>>;

class Gem : public GameObject
{
private:
	SpriteRenderer* _spriteRenderer;
	TextRenderer* _textRenderer;
	jgl::SpriteSheet* _texture;

	void _prepareBakeSprite()
	{
		Mesh newMesh;

		newMesh.texture = _texture;

		static jgl::UInt element_index[6] = { 0, 3, 1, 2, 3, 0 };
		static jgl::Vector3 deltaPosition[4] = {
			jgl::Vector3(0, 0, 1),
			jgl::Vector3(1, 0, 1),
			jgl::Vector3(0, 1, 1),
			jgl::Vector3(1, 1, 1)
		};
		static jgl::Vector2 deltaUvs[4] = {
			jgl::Vector2(0, 0),
			jgl::Vector2(1, 0),
			jgl::Vector2(0, 1),
			jgl::Vector2(1, 1)
		};

		for (jgl::Size_t i = 0; i < 4; i++)
		{
			newMesh.vertices.push_back(core().size * deltaPosition[i]);
			newMesh.uvs.push_back(_texture->unit() * deltaUvs[i]);
		}
		for (jgl::Size_t i = 0; i < 6; i++)
		{
			newMesh.elements.push_back(element_index[i]);
		}

		_spriteRenderer->setMesh(newMesh);
	}

	void _prepareBakeText()
	{
		TextMesh newMesh;

		newMesh.text = "aO";
		newMesh.horizontalAlignment = jgl::HorizontalAlignment::Centred;
		newMesh.verticalAlignment = jgl::VerticalAlignment::Centred;
		newMesh.textColor = jgl::Color(255, 255, 255);
		newMesh.textWorldHeight = 1.0f;
		newMesh.outlineColor = jgl::Color(0, 0, 0);
		newMesh.font = jgl::Application::instance()->defaultFont();
		newMesh.textWorldArea = jgl::Vector2(2, 2);

		_textRenderer->setMesh(newMesh);
	}

public:
	Gem(std::string p_name, GameObject* p_parent = nullptr) : GameObject(p_name, p_parent)
	{
		_spriteRenderer = addComponent<SpriteRenderer>();
		_textRenderer = addComponent<TextRenderer>();
	}

	void prepareBake()
	{
		_prepareBakeSprite();
		_prepareBakeText();

	}
	void setTexture(jgl::SpriteSheet* p_texture)
	{
		_texture = p_texture;
	}
};

class SceneRenderer : public jgl::Widget
{
private:
	jgl::SpriteSheet* _gameObjectSpriteSheet;

	Scene* _scene;

	Gem* _object;

	jgl::Bool _onUpdate()
	{
		DebugScreen::instance()->setText("Ceci est un test", 1, 16);
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Z) == jgl::InputStatus::Released)
		{
			_scene->mainCamera()->core().position += jgl::Vector3(0, 1, 0);
			_scene->mainCamera()->unbake();
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Q) == jgl::InputStatus::Released)
		{
			_scene->mainCamera()->core().position += jgl::Vector3(-1, 0, 0);
			_scene->mainCamera()->unbake();
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::S) == jgl::InputStatus::Released)
		{
			_scene->mainCamera()->core().position += jgl::Vector3(0, -1, 0);
			_scene->mainCamera()->unbake();
		}
		if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::D) == jgl::InputStatus::Released)
		{
			_scene->mainCamera()->core().position += jgl::Vector3(1, 0, 0);
			_scene->mainCamera()->unbake();
		}
		_scene->update();
		return (false);
	}

	void _onRender()
	{
		_scene->render();
	}

	void _onGeometryChange()
	{
		jgl::Float cellOnX = size().x() / 64;
		jgl::Float ratio = static_cast<jgl::Float>(size().y()) / static_cast<jgl::Float>(size().x());
		_scene->mainCamera()->setViewportSize(jgl::Vector2(cellOnX, ratio * cellOnX));
		_object->getComponent<SpriteRenderer>()->unbake();
		_object->getComponent<TextRenderer>()->unbake();
	}

public:
	SceneRenderer(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_gameObjectSpriteSheet = new jgl::SpriteSheet("gameObjectSpriteSheet.png", jgl::Vector2Int(4, 1));

		_scene = new Scene();
		_scene->setMainCamera(new Camera("MainCamera"));

		_scene->mainCamera()->core().position = jgl::Vector3(0, 0, 10);
		_scene->mainCamera()->core().forward = jgl::Vector3(0, 0, -1);

		_object = new Gem("GemA");
		_object->setTexture(_gameObjectSpriteSheet);
		_object->core().position = jgl::Vector3(0, 0, 0);
		_object->core().size = jgl::Vector2(2, 2);
		_object->prepareBake();

		_scene->addGameObject(_object);
	}
};

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("ProjectName", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("karma suture.ttf"));
	app.deactivateMultiThread();

	DebugScreen::instanciate(nullptr);
	DebugScreen::instance()->setGeometry(jgl::Vector2Int(0, 0), app.size());
	DebugScreen::instance()->activate();

	SceneRenderer* sceneRenderer = new SceneRenderer(nullptr);
	sceneRenderer->setGeometry(jgl::Vector2Int(0, 0), app.size());
	sceneRenderer->activate();

	return (app.run());
}