#pragma once

#include "Structure/Widget/jgl2_widget.h"

namespace jgl
{
	template <typename TTilemap>
	class Abstract2DTilemapRenderer : public jgl::Widget
	{
	protected:
		virtual jgl::Vector2 _getCameraPosition() = 0;

		jgl::Vector2Int _convertWorldToScreen(jgl::Vector2 p_pos)
		{
			jgl::Vector2 player_pos = _getCameraPosition();

			jgl::Vector2 result = p_pos;

			result -= jgl::Vector2(0.5f, 0.5f);
			result -= player_pos;
			result *= jgl::Vector2Int(TTilemap::ChunkType::NodeType::SIZE, TTilemap::ChunkType::NodeType::SIZE);
			result += anchor() + size() / jgl::Vector2Int(2, 2);

			return (result);
		}
		jgl::Vector2 _convertScreenToWorld(jgl::Vector2Int p_pos)
		{
			jgl::Vector2 player_pos = _getCameraPosition();

			jgl::Vector2 result = p_pos;

			result -= anchor() + size() / jgl::Vector2Int(2, 2);
			result /= jgl::Vector2Int(TTilemap::ChunkType::NodeType::SIZE, TTilemap::ChunkType::NodeType::SIZE);
			result += player_pos;
			result += jgl::Vector2(0.5f, 0.5f);

			result = result.floor();

			return (result);
		}
		jgl::Vector2 _convertScreenToChunk(jgl::Vector2Int p_pos)
		{
			jgl::Vector2 player_pos = _getCameraPosition();

			jgl::Vector2 result = p_pos;

			result -= jgl::Application::instance()->size() / jgl::Vector2Int(2, 2);
			result /= jgl::Vector2Int(TTilemap::ChunkType::NodeType::SIZE, TTilemap::ChunkType::NodeType::SIZE);
			result += player_pos;
			result += jgl::Vector2(0.5f, 0.5f);

			result = result.floor();

			result /= jgl::Vector2Int(TTilemap::ChunkType::C_SIZE, TTilemap::ChunkType::C_SIZE);

			result = result.floor();

			return (result);
		}

	public:
		Abstract2DTilemapRenderer(jgl::Widget* p_parent) : jgl::Widget(p_parent)
		{

		}
	};
}