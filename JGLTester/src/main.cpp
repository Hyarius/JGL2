#include "jgl2_game_engine.h"

class Gem : public GameObject
{
private:
	SpriteRenderer* _spriteRenderer;
	jgl::SpriteSheet* _texture;

public:
	Gem(std::string p_name, GameObject* p_parent = nullptr) : GameObject(p_name, p_parent)
	{
		_spriteRenderer = addComponent<SpriteRenderer>();
	}

	void prepareBake()
	{
		_spriteRenderer->mesh().texture = _texture;

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
			_spriteRenderer->mesh().vertices.push_back(core().position + core().size * deltaPosition[i]);
			_spriteRenderer->mesh().uvs.push_back(_texture->unit() * deltaUvs[i]);
		}
		for (jgl::Size_t i = 0; i < 6; i++)
		{
			_spriteRenderer->mesh().elements.push_back(element_index[i]);
		}
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

	Gem* _objects[4];

	jgl::Bool _onUpdate()
	{
		_scene->update();
		return (false);
	}
	
	void _onRender()
	{
		_scene->render();
	}
	
	void _onGeometryChange()
	{
		jgl::Float cellOnX = size().x() / 32;
		jgl::Float ratio = static_cast<jgl::Float>(size().y()) / static_cast<jgl::Float>(size().x());
		_scene->mainCamera()->setViewportSize(jgl::Vector2(cellOnX, ratio * cellOnX));
		for (jgl::Size_t i = 0; i < 4; i++)
		{
			_objects[i]->getComponent<SpriteRenderer>()->unbake();
		}
	}

public:
	SceneRenderer(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_gameObjectSpriteSheet = new jgl::SpriteSheet("gameObjectSpriteSheet.png", jgl::Vector2Int(4, 1));

		_scene = new Scene();
		_scene->setMainCamera(new Camera("MainCamera"));

		_scene->mainCamera()->core().position = jgl::Vector3(0, 0, 10);
		_scene->mainCamera()->core().forward = jgl::Vector3(0, 0, -1);

		for (jgl::Size_t i = 0; i < 4; i++)
		{
			_objects[i] = new Gem("GemA");
			_objects[i]->setTexture(_gameObjectSpriteSheet);
			_objects[i]->core().position = jgl::Vector3(jgl::generateNumber(-5, 5), jgl::generateNumber(-5, 5), jgl::generateNumber(0, 5));
			_objects[i]->core().size = jgl::Vector2(2, 2);
			_objects[i]->prepareBake();

			_scene->addGameObject(_objects[i]);
		}
	}
};

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("ProjectName", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("karma suture.ttf"));
	app.deactivateMultiThread();

	SceneRenderer* sceneRenderer = new SceneRenderer(nullptr);
	sceneRenderer->setGeometry(jgl::Vector2Int(0, 0), app.size());
	sceneRenderer->activate();

	return (app.run());
}