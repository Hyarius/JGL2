#include "jgl2.h"

class MapRenderer : public jgl::Widget
{
	using Node = jgl::INode;
	using Chunk = jgl::BakableChunk2D<Node, 16, 2>; 
	using Tilemap = jgl::Tilemap<Chunk>;
private:
	jgl::SpriteSheet* _spriteSheet = nullptr;
	Tilemap* _tilemap = nullptr;

	jgl::Vector2Int convertWorldToScreen(jgl::Vector2 p_pos);
	jgl::Vector2 convertScreenToWorld(jgl::Vector2Int p_pos);
	jgl::Vector2 convertScreenToChunk(jgl::Vector2Int p_pos);

	jgl::Bool _onUpdate();
	void _onRender();
	void _onGeometryChange();

public:
	MapRenderer(jgl::Widget* p_parent);
};

jgl::Vector2Int MapRenderer::convertWorldToScreen(jgl::Vector2 p_pos)
{
	jgl::Vector2 player_pos = jgl::Vector2(0, 0);

	jgl::Vector2 result = p_pos;

	result -= jgl::Vector2(0.5f, 0.5f);
	result -= player_pos;
	result *= jgl::Vector2Int(Node::SIZE, Node::SIZE);
	result += anchor() + size() / jgl::Vector2Int(2, 2);

	return (result);
}

jgl::Vector2 MapRenderer::convertScreenToWorld(jgl::Vector2Int p_pos)
{
	jgl::Vector2 player_pos = jgl::Vector2(0, 0);

	jgl::Vector2 result = p_pos;

	result -= anchor() + size() / jgl::Vector2Int(2, 2);
	result /= jgl::Vector2Int(Node::SIZE, Node::SIZE);
	result += player_pos;
	result += jgl::Vector2(0.5f, 0.5f);

	result = result.floor();

	return (result);
}

jgl::Vector2 MapRenderer::convertScreenToChunk(jgl::Vector2Int p_pos)
{
	jgl::Vector2 player_pos = jgl::Vector2(0, 0);

	jgl::Vector2 result = p_pos;

	result -= jgl::Application::instance()->size() / jgl::Vector2Int(2, 2);
	result /= jgl::Vector2Int(Node::SIZE, Node::SIZE);
	result += player_pos;
	result += jgl::Vector2(0.5f, 0.5f);

	result = result.floor();

	result /= jgl::Vector2Int(Chunk::C_SIZE, Chunk::C_SIZE);

	result = result.floor();
	 
	return (result);
}

void MapRenderer::_onRender()
{
	jgl::Vector2Int start = convertScreenToWorld(jgl::Vector2Int(0, 0));
	jgl::Vector2Int end = convertScreenToWorld(size());

	for (jgl::Int x = start.x(); x <= end.x(); x++)
	{
		for (jgl::Int y = start.y(); y <= end.y(); y++)
		{
			Chunk* tmp = _tilemap->chunk(jgl::Vector2Int(x, y));
			if (tmp != nullptr)
			{ 
				if (tmp->baked() == false)
				{
					tmp->bake<Chunk>(_tilemap, false);
				}

				jgl::Vector3 tmp_anchor = jgl::Vector3(
					jgl::Application::instance()->convertScreenToOpenGL(
						convertWorldToScreen(tmp->pos() * jgl::Vector2Int(Chunk::C_SIZE, Chunk::C_SIZE))
						),
					depth());
				tmp->render(
					jgl::Application::instance()->convertScreenToOpenGL(convertWorldToScreen(tmp->pos() * jgl::Vector2Int(Chunk::C_SIZE, Chunk::C_SIZE))),
					depth(), 
					0
				);
			}
		}
	}
	
}

void MapRenderer::_onGeometryChange()
{
	Node::SIZE = 32;
	_tilemap->unbake();
}

jgl::Bool MapRenderer::_onUpdate()
{
	static const jgl::Vector2Int deltaChunkPos[9] = {
		 jgl::Vector2Int(-1, -1), jgl::Vector2Int(0, -1), jgl::Vector2Int(1, -1),
		 jgl::Vector2Int(-1,  0), jgl::Vector2Int(0,  0), jgl::Vector2Int(1,  0),
		 jgl::Vector2Int(-1,  1), jgl::Vector2Int(0,  1), jgl::Vector2Int(1,  1),
	};

	if (jgl::Application::instance()->mouse().button(jgl::Mouse::Button::Left) == jgl::InputStatus::Released)
	{
		jgl::Vector2Int worldPos = convertScreenToWorld(jgl::Application::instance()->mouse().pos());
		
		jgl::Vector2Int chunkPos = _tilemap->convertWorldToChunk(worldPos);
		_tilemap->requestChunk(chunkPos);
		_tilemap->setContent(worldPos, 1, 4); 
		for (jgl::Size_t i = 0; i < 9; i++)
		{
			Chunk* tmp_chunk = _tilemap->chunk(chunkPos + deltaChunkPos[i]);

			if (tmp_chunk != nullptr)
			{
				tmp_chunk->unbake();
			}
		}
	}
	return (false);
}

MapRenderer::MapRenderer(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_spriteSheet = new jgl::SpriteSheet("tilemapSprite.png", jgl::Vector2Int(5, 6));
	_tilemap = new Tilemap();

	Chunk::addNode(new Node(0, jgl::Vector2Int(0, 0), true, false, 1));
	Chunk::addNode(new Node(1, jgl::Vector2Int(4, 0), false, false, 1));
	Chunk::addNode(new Node(2, jgl::Vector2Int(4, 1), false, false, 1));
	Chunk::addNode(new Node(3, jgl::Vector2Int(4, 2), false, false, 1));
	Chunk::addNode(new Node(4, jgl::Vector2Int(4, 3), false, false, 1));
	Chunk::addNode(new Node(5, jgl::Vector2Int(4, 4), false, false, 1));
	Chunk::addNode(new Node(6, jgl::Vector2Int(4, 5), false, false, 1));
	Chunk::setNodeTexture(_spriteSheet);
	_tilemap->addChunk(jgl::Vector2Int(0, 0), new Chunk(jgl::Vector2Int(0, 0)));
	_tilemap->setContent(jgl::Vector2Int(0, 0), 0);
	_tilemap->setContent(jgl::Vector2Int(1, 0), 0);
	_tilemap->setContent(jgl::Vector2Int(1, 0), 1, 4);
	_tilemap->setContent(jgl::Vector2Int(0, 1), 0);
	_tilemap->setContent(jgl::Vector2Int(1, 1), 0);
	_tilemap->setContent(jgl::Vector2Int(2, 0), 1);
	_tilemap->setContent(jgl::Vector2Int(2, 1), 1);
	_tilemap->setContent(jgl::Vector2Int(0, 2), 2);
	_tilemap->setContent(jgl::Vector2Int(1, 2), 2);
	_tilemap->setContent(jgl::Vector2Int(2, 2), 3);
}

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("Erelia", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	app.activateMultiThread();
	app.setDefaultFont(new jgl::Font("karma suture.ttf"));

	MapRenderer* _mapRenderer = new MapRenderer(nullptr);
	_mapRenderer->setGeometry(jgl::Vector2Int(0, 0), app.size());
	_mapRenderer->activate();

	return (app.run());
}