#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

#include "structure/jgl2_vector.h"
#include "structure/jgl2_color.h"
#include "structure/jgl2_image.h"

namespace jgl
{
	class Font : public jgl::Image_handler
	{
	private:
		friend class jgl::Application;

		struct Glyph_data
		{
			jgl::Size_t height;
			jgl::Vector2 positions[4];
			jgl::Vector2 uvs[4];
			jgl::Vector2 offset = 0;
		};

		jgl::Map<jgl::Glyph, Glyph_data> _atlas;
		jgl::Array<jgl::Color> _color_buffer;
		jgl::Array<jgl::Color> _color_outline_buffer;

		Glyph_data& _get_data(jgl::Glyph to_draw);

		void _clear()
		{
			_vertex_buffer.clear();
			_uvs_buffer.clear();
			_alpha_buffer.clear();
			_element_buffer.clear();
			_color_buffer.clear();
			_color_outline_buffer.clear();
		}
		
	public:
		/*
			Constructor
			Create a font with a file located at [path] location
		*/
		Font(jgl::String path);
		/*
			Default destructor
		*/
		~Font();

		/*
			Draw the whole font texture with white glyph with black outline
		*/
		void draw(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float depth, jgl::Float alpha, jgl::Viewport* p_viewport = nullptr);

		/*
			Draw a string [text] on screen from top-left corner at pos [pos] and with a glyph vertical size of [size] (in pixel)
			[depth] describ the depth on which this text will be drawn on screen
			[alpha] represent the transparency of the text, expres from 0 to 1 - 0 is fully transparent, 1 is opaque
			The text will be print in full color [text_color], with no border
		*/
		jgl::Vector2Int draw_text(jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float depth, jgl::Float alpha, jgl::Color text_color, jgl::Viewport* p_viewport = nullptr);

		/*
			Draw a glyph [to_draw] on screen from top-left corner at pos [pos] and with a glyph vertical size of [size] (in pixel)
			[depth] describ the depth on which this text will be drawn on screen
			[alpha] represent the transparency of the text, expres from 0 to 1 - 0 is fully transparent, 1 is opaque
			The text will be print in full color [text_color], with no border
		*/
		jgl::Vector2Int draw_char(jgl::Glyph to_draw, jgl::Vector2Int pos, jgl::Uint size, jgl::Float depth, jgl::Float alpha, jgl::Color text_color, jgl::Viewport* p_viewport = nullptr);

		/*
			Draw a string [text] on screen from top-left corner at pos [pos] and with a glyph vertical size of [size] (in pixel)
			[depth] describ the depth on which this text will be drawn on screen
			[alpha] represent the transparency of the text, expres from 0 to 1 - 0 is fully transparent, 1 is opaque
			The text will be print in color [text_color], with a border of [outline_color] color
		*/
		jgl::Vector2Int draw_text(jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float depth, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color, jgl::Viewport* p_viewport = nullptr);

		/*
			Draw a glyph [to_draw] on screen from top-left corner at pos [pos] and with a glyph vertical size of [size] (in pixel)
			[depth] describ the depth on which this text will be drawn on screen
			[alpha] represent the transparency of the text, expres from 0 to 1 - 0 is fully transparent, 1 is opaque
			The text will be print in color [text_color], with a border of [outline_color] color
		*/
		jgl::Vector2Int draw_char(jgl::Glyph to_draw, jgl::Vector2Int pos, jgl::Uint size, jgl::Float depth, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color, jgl::Viewport* p_viewport = nullptr);

		/*
			Calculate the horizontal size of a string [text] with desired vertical [size] 
		*/
		jgl::Vector2Int calc_text_size(jgl::String text, jgl::Uint size);

		/*
			Calculate the horizontal size of a glyph [to_draw] with desired vertical [size]
		*/
		jgl::Vector2Int calc_char_size(jgl::Glyph to_draw, jgl::Uint size);

		/*
			Calculate the horizontal offset of a glyph [to_draw] with desired vertical [size]
		*/
		jgl::Vector2Int calc_char_offset(jgl::Glyph text, jgl::Uint size);

	};
};