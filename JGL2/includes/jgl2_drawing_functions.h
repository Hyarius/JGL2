#pragma once

#include "jgl2_includes.h"

#include "structure/jgl2_color.h"
#include "structure/jgl2_vector.h"

namespace jgl
{
	/*
		Draw ([size] / 3) triangle with plain [color] to the screen, at pos describes by [points]  (in screen coordinate system) 
			[depth] represent the depth on which the image will be drawn on screen. 0 is draw behind 1, etc
	*/
	void draw_triangle_color(jgl::Vector2Int* points, jgl::Size_t size, jgl::Color color, jgl::Float depth, jgl::Viewport* viewport = nullptr);

	/*
		Draw one rectangle with plain [color] to the screen, with top-left corner at position [pos] (in pixel), and with a size of [size] pixel
		[depth] represent the depth on which the image will be drawn on screen. 0 is draw behind 1, etc
	*/
	void draw_rectangle_color(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Color color, jgl::Float depth, jgl::Viewport* viewport = nullptr);
}
