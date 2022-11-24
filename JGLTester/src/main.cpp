#include "jgl2.h"

class MapRenderer : public jgl::Widget
{
	using Node = jgl::INode;
	using Chunk = jgl::IBakableChunk<Node, 16, 1>;
	using Tilemap = jgl::Tilemap<Chunk>;
private:
	jgl::SpriteSheet* _spriteSheet = nullptr;
	Tilemap* _tilemap = nullptr;

	jgl::Bool _onUpdate();
	void _onRender();
	void _onGeometryChange();

public:
	MapRenderer(jgl::Widget* p_parent);
};

void MapRenderer::_onRender()
{
	Chunk* tmp = _tilemap->chunk(jgl::Vector2Int(0, 0));
	if (tmp != nullptr)
	{
		if (tmp->baked() == false)
			tmp->bake<Chunk>(_tilemap, false);

		tmp->render(jgl::Vector3(0, 0, 0), 0);
	}
}

void MapRenderer::_onGeometryChange()
{

}

jgl::Bool MapRenderer::_onUpdate()
{
	return (false);
}

MapRenderer::MapRenderer(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	DEBUG_LINE();
	_spriteSheet = new jgl::SpriteSheet("tilemapSprite.png", jgl::Vector2Int(2, 2));
	_tilemap = new Tilemap();
	DEBUG_LINE();

	Chunk::addNode(new Node(0, jgl::Vector2Int(0, 0), false, false, 1));
	Chunk::addNode(new Node(1, jgl::Vector2Int(1, 0), false, false, 1));
	Chunk::addNode(new Node(2, jgl::Vector2Int(0, 1), false, false, 1));
	Chunk::addNode(new Node(3, jgl::Vector2Int(1, 1), false, false, 1));
	DEBUG_LINE();
	Chunk::setNodeTexture(_spriteSheet);
	DEBUG_LINE();
	_tilemap->addChunk(jgl::Vector2Int(0, 0), new Chunk(jgl::Vector2Int(0, 0)));
	DEBUG_LINE();
	_tilemap->setContent(jgl::Vector2Int(0, 0), 0);
	_tilemap->setContent(jgl::Vector2Int(1, 0), 0);
	_tilemap->setContent(jgl::Vector2Int(0, 1), 0);
	_tilemap->setContent(jgl::Vector2Int(1, 1), 0);
	_tilemap->setContent(jgl::Vector2Int(2, 0), 1);
	_tilemap->setContent(jgl::Vector2Int(2, 1), 1);
	_tilemap->setContent(jgl::Vector2Int(0, 2), 2);
	_tilemap->setContent(jgl::Vector2Int(1, 2), 2);
	_tilemap->setContent(jgl::Vector2Int(2, 2), 3);
	DEBUG_LINE();
}

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("Erelia", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	app.activateMultiThread();
	app.setDefaultFont(new jgl::Font("karma suture.ttf"));

	DEBUG_LINE();
	MapRenderer* _mapRenderer = new MapRenderer(nullptr);
	DEBUG_LINE();
	_mapRenderer->setGeometry(jgl::Vector2Int(0, 0), app.size());
	DEBUG_LINE();
	_mapRenderer->activate();
	DEBUG_LINE();

	return (app.run());
}