#include "Structure/jgl2_sprite_sheet.h"

namespace jgl
{
	SpriteSheet::SpriteSheet(std::string path, Vector2UInt p_size) : Image(path)
	{
		_size = p_size;
		_unit = Vector2(1.0f, 1.0f) / _size;

		if (p_size == Vector2UInt(1, 1))
			_sprites.push_back(Vector2(0, 0));
		else
		{
			for (Size_t j = 0; j < _size.y(); j++)
			{
				for (Size_t i = 0; i < _size.x(); i++)
				{
					_sprites.push_back(Vector2(i, j) * _unit);
				}
			}
		}
	}

	const Vector2UInt& SpriteSheet::size() const
	{
		return (_size);
	}

	const Vector2& SpriteSheet::unit() const
	{
		return (_unit);
	}

	const std::vector<Vector2>& SpriteSheet::sprites() const
	{
		return (_sprites);
	}

	const Vector2& SpriteSheet::sprite(Size_t index) const
	{
		if (_sprites.size() <= index)
			throw std::runtime_error("Segfault : sprite id out of range");
		return (_sprites[index]);
	}

	const Vector2& SpriteSheet::sprite(Vector2UInt sprite_value) const
	{
		UInt sprite_id = (_size.x() * sprite_value.y()) + sprite_value.x();
		return (sprite(sprite_id));
	}

	const Vector2& SpriteSheet::sprite(Vector2Int sprite_value) const
	{
		UInt sprite_id = (_size.x() * sprite_value.y()) + sprite_value.x();
		return (sprite(sprite_id));
	}

	void SpriteSheet::draw(UInt sprite_id, Vector2Int pos, Vector2UInt size, jgl::Float p_depth)
	{
		jgl::Image::draw(pos, size, _sprites[sprite_id], _unit, p_depth);
	}

	void SpriteSheet::draw(Vector2Int sprite, Vector2Int pos, Vector2UInt size, jgl::Float p_depth)
	{
		UInt sprite_id = (_size.x() * sprite.y()) + sprite.x();
		this->draw(sprite_id, pos, size, p_depth);
	}
}