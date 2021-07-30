#pragma once

#include "structure/jgl2_array.h"
#include "structure/jgl2_image.h"
#include "jgl2_drawing_functions.h"

class Sprite_sheet : public jgl::Image
{
private:
	jgl::Vector2Uint _size;
	jgl::Vector2 _unit;
	jgl::Array<jgl::Vector2> _sprites;
	
public:
	/*
		Constructor from an image, with desired number of sprite on X/Y
	*/
	Sprite_sheet(jgl::String path, jgl::Vector2Uint p_size) : jgl::Image(path)
	{
		_size = p_size;

		jgl::Vector2	tmp = 0;
		if (p_size == 1)
			_sprites.push_back(0);
		else
		{
			while (tmp.y != _size.y)
			{
				jgl::Vector2 result = jgl::Vector2(tmp.x / _size.x, tmp.y / _size.y);
				_sprites.push_back(result);
				tmp.x++;
				if (tmp.x >= _size.x)
				{
					tmp.x = 0;
					tmp.y++;
				}
			}
		}
	}

	/*
		Draw the sprite number [sprite_id] of the sprite_sheet on screen, at position [pos] with size [size].
		[depth] represent the depth on which the image will be drawn on screen. 0 is draw behind 1, etc
		[alpha] represent the transparency of the image, express from 0 to 1. 0 mean fully transparent, 1 fully opaque
	*/
	void draw(jgl::Uint sprite_id, jgl::Vector2Int pos, jgl::Vector2Int size,
		jgl::Float depth, jgl::Float alpha, const jgl::Viewport* viewport)
	{
		jgl::Size_t base_size = _vertex_buffer.size();
		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };
		static jgl::Vector2Int delta_pos[4] = {
			jgl::Vector2Int(0, 0),
			jgl::Vector2Int(1, 0),
			jgl::Vector2Int(0, 1),
			jgl::Vector2Int(1, 1)
		};

		for (size_t i = 0; i < 4; i++)
		{
			_vertex_buffer.push_back(convert_screen_to_opengl(pos + size * delta_pos[i], depth));
			_uvs_buffer.push_back(_sprites[sprite_id] + _unit * delta_pos[i]);
			_alpha_buffer.push_back(alpha);
		}

		for (size_t i = 0; i < 6; i++)
			_element_buffer.push_back(element_index[i] + base_size);
	}


	/*
		Draw the sprite at position [sprite] inside the image of the sprite_sheet on screen, at position [pos] with size [size].
		[depth] represent the depth on which the image will be drawn on screen. 0 is draw behind 1, etc
		[alpha] represent the transparency of the image, express from 0 to 1. 0 mean fully transparent, 1 fully opaque
	*/
	void draw(jgl::Vector2Int sprite, jgl::Vector2Int pos, jgl::Vector2Int size,
		jgl::Float depth, jgl::Float alpha, const jgl::Viewport* viewport)
	{
		jgl::Size_t base_size = _vertex_buffer.size();
		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };
		static jgl::Vector2Int delta_pos[4] = {
			jgl::Vector2Int(0, 0),
			jgl::Vector2Int(1, 0),
			jgl::Vector2Int(0, 1),
			jgl::Vector2Int(1, 1)
		};

		jgl::Uint sprite_id = (_size.x * sprite.y) + sprite.x;
		for (size_t i = 0; i < 4; i++)
		{
			_vertex_buffer.push_back(convert_screen_to_opengl(pos + size * delta_pos[i], depth));
			_uvs_buffer.push_back(_sprites[sprite_id] + _unit * delta_pos[i]);
			_alpha_buffer.push_back(alpha);
		}

		for (size_t i = 0; i < 6; i++)
			_element_buffer.push_back(element_index[i] + base_size);
	}
};