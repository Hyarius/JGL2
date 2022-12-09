#pragma once

#include "Structure/jgl2_vector.h"
#include "Structure/jgl2_image.h"

namespace jgl
{
	class SpriteSheet : public Image
	{
	private:
		Vector2UInt _size;
		Vector2 _unit;
		std::vector<Vector2> _sprites;

	public:
		SpriteSheet(std::string path, Vector2UInt p_size);

		const Vector2UInt& size() const;
		const Vector2& unit() const;
		const std::vector<Vector2>& sprites() const;
		const Vector2& sprite(Size_t index) const;
		const Vector2& sprite(Vector2UInt sprite_value) const;
		const Vector2& sprite(Vector2Int sprite_value) const;

		void draw(UInt sprite_id, Vector2Int pos, Vector2UInt size, jgl::Float p_depth);
		void draw(Vector2Int sprite, Vector2Int pos, Vector2UInt size, jgl::Float p_depth);
	};
}