#include "jgl2.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "external_lib/std_truetype.h"

extern jgl::Array<jgl::Font*>* g_application_font_array;

namespace jgl
{
	Font::Font(jgl::String path) : jgl::Image_handler()
	{
		jgl::Long size;
		jgl::Uchar* fontBuffer;

		if (jgl::check_file_exist(path) == false)
			THROW_EXCEPTION(jgl::Error_level::Error, 1, "Try to parse file " + path + " to load a font : file doesn't exist");

		FILE* fontFile = fopen(path.c_str(), "rb");
		fseek(fontFile, 0, SEEK_END);
		size = ftell(fontFile); /* how long is the file ? */
		fseek(fontFile, 0, SEEK_SET); /* reset */

		fontBuffer = (jgl::Uchar *)malloc(size);

		fread(fontBuffer, size, 1, fontFile);
		fclose(fontFile);

		const jgl::Size_t nb_char = 256;
		const jgl::Int oversample_x = 3;
		const jgl::Int oversample_y = 3;

		const jgl::Size_t char_size = 90;

		jgl::Uchar* atlasData;

		jgl::Int width = 32;
		jgl::Int height = 32;

		stbtt_packedchar* char_info = new stbtt_packedchar[nb_char];

		while (1) {
			atlasData = new jgl::Uchar[width * height];

			stbtt_pack_context context;
			if (!stbtt_PackBegin(&context, atlasData, width, height, 0, 1, nullptr))
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "Failed to initialize font");

			stbtt_PackSetOversampling(&context, 1, 1);
			if (!stbtt_PackFontRange(&context, fontBuffer, 0, char_size, 0, nb_char, char_info)) {
				// too small
				delete [] atlasData;
				stbtt_PackEnd(&context);
				width *= 2;
				height *= 2;
			}
			else {
				stbtt_PackEnd(&context);
				break;
			}
		}

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, atlasData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		jgl::Float width_delta = 0.5f / width;
		jgl::Float height_delta = 0.5f / height;
		for (jgl::Size_t i = 0; i < nb_char; i++)
		{
			jgl::Glyph c = i;
			Glyph_data data;

			stbtt_aligned_quad quad;

			stbtt_GetPackedQuad(char_info, width, height, c.value(), &data.offset.x, &data.offset.y, &quad, 1);
			const jgl::Float xmin = quad.x0;
			const jgl::Float xmax = quad.x1;
			const jgl::Float ymin = -quad.y1;
			const jgl::Float ymax = -quad.y0;

			data.height = char_size;
			data.positions[0] = jgl::Vector2(xmin / char_size, ymin / char_size);
			data.positions[1] = jgl::Vector2(xmax / char_size, ymin / char_size);
			data.positions[2] = jgl::Vector2(xmin / char_size, ymax / char_size);
			data.positions[3] = jgl::Vector2(xmax / char_size, ymax / char_size);
			data.offset /= char_size;

			data.uvs[0] = { quad.s0 - width_delta, quad.t0 - height_delta };
			data.uvs[1] = { quad.s1 + width_delta, quad.t0 - height_delta };
			data.uvs[2] = { quad.s0 - width_delta, quad.t1 + height_delta };
			data.uvs[3] = { quad.s1 + width_delta, quad.t1 + height_delta };

			_atlas[c] = data;
		}

		_program = jgl::Application::active_application()->shader("Texture_text_shader_2D");
		g_application_font_array->push_back(this);
		THROW_INFORMATION("Font created successfully with truetypefile [" + path + "] at id [" + jgl::itoa(_id) + "]");
	}

	Font::~Font()
	{
		if (g_application_font_array == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "No application started while deleting an image");
		auto it = std::find(g_application_font_array->begin(), g_application_font_array->end(), this);
		if (it != g_application_font_array->end())
			g_application_font_array->erase(it);
	}

	Font::Glyph_data& Font::_get_data(jgl::Glyph to_draw)
	{
		if (_atlas.count(to_draw) == 0)
			THROW_EXCEPTION(jgl::Error_level::Error, 1, "Trying to print unespected glyph");
		return (_atlas[to_draw]);
	}
	void Font::draw(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Float level, jgl::Float alpha, jgl::Viewport* p_viewport)
	{
		jgl::Uint old_len = _vertex_buffer.size();

		jgl::Vector2 glyph_size = 1;

		jgl::Vector3 converted_pos[4] = {
			jgl::convert_screen_to_opengl(pos + size * jgl::Vector2Int(0, 0), level),
			jgl::convert_screen_to_opengl(pos + size * jgl::Vector2Int(1, 0), level),
			jgl::convert_screen_to_opengl(pos + size * jgl::Vector2Int(0, 1), level),
			jgl::convert_screen_to_opengl(pos + size * jgl::Vector2Int(1, 1), level),
		};

		jgl::Vector2 uvs_pos[4] = {
			jgl::Vector2Int(0, 0),
			jgl::Vector2Int(1, 0),
			jgl::Vector2Int(0, 1),
			jgl::Vector2Int(1, 1),
		};

		for (jgl::Size_t i = 0; i < 4; i++)
		{
			_vertex_buffer.push_back(converted_pos[i]);
			_color_buffer.push_back(jgl::Color::white());
			_color_outline_buffer.push_back(jgl::Color::black());
			_uvs_buffer.push_back(uvs_pos[i]);
			_alpha_buffer.push_back(alpha);
		}

		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };

		for (jgl::Size_t i = 0; i < 6; i++)
		{
			_element_buffer.push_back(element_index[i] + old_len);
		}
	}
	jgl::Vector2Int Font::draw_text(jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Viewport* p_viewport)
	{
		return (draw_text(text, pos, size, level, alpha, text_color, text_color, p_viewport));
	}

	jgl::Vector2Int Font::draw_text(jgl::String text, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color outline_color, jgl::Viewport* p_viewport)
	{
		jgl::Vector2Int delta = 0;
		for (jgl::Size_t i = 0; i < text.size(); i++)
		{
			delta.x += draw_char(text[i], pos + delta, size, level + 0.1f, alpha, text_color, outline_color, p_viewport).x;
		}
		delta.y = size;
		return (delta);
	}

	jgl::Vector2Int Font::calc_text_size(jgl::String text, jgl::Uint size)
	{
		jgl::Vector2Int delta = 0;
		delta.y = size;
		for (jgl::Size_t i = 0; i < text.size(); i++)
		{
			Font::Glyph_data glyph_data = _get_data(text[i]);
			delta.x += (glyph_data.offset * size).x;
		}
		return (delta);
	}

	jgl::Vector2Int Font::calc_char_offset(jgl::Glyph to_draw, jgl::Uint size)
	{
		Glyph_data& data = _get_data(to_draw);
		jgl::Vector2 tmp = data.offset;
		tmp *= (size);
		return (tmp);
	}

	jgl::Vector2Int Font::calc_char_size(jgl::Glyph to_draw, jgl::Uint size)
	{
		Glyph_data& data = _get_data(to_draw);
		jgl::Vector2 tmp = (data.positions[3] - data.positions[0]);
		tmp *= (size);
		return (tmp);
	}

	jgl::Vector2Int Font::draw_char(jgl::Glyph to_draw, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Viewport* p_viewport)
	{
		return (draw_char(to_draw, pos, size, level, alpha, text_color, text_color, p_viewport));
	}

	jgl::Vector2Int Font::draw_char(jgl::Glyph to_draw, jgl::Vector2Int pos, jgl::Uint size, jgl::Float level, jgl::Float alpha, jgl::Color text_color, jgl::Color text_outline_color, jgl::Viewport* p_viewport)
	{
		if (p_viewport != nullptr)
			p_viewport->use();

		jgl::Vector2Int tmp_size = calc_char_size(to_draw, size);
		jgl::Uint old_len = _vertex_buffer.size();

		Font::Glyph_data& glyph_data = _get_data(to_draw);

		jgl::Vector2Int delta[4] = {
			{0, 0},
			{1, 0},
			{0, 1},
			{1, 1},
		};

		jgl::Vector2Int delta_pos = jgl::Vector2(0, size - tmp_size.y);
		if (to_draw == '\'')
			delta_pos = jgl::Vector2(0, tmp_size.y);

		for (jgl::Size_t i = 0; i < 4; i++)
		{
			_vertex_buffer.push_back(jgl::convert_screen_to_opengl(pos + delta_pos + tmp_size * delta[i], level));
			_color_buffer.push_back(text_color);
			_color_outline_buffer.push_back(text_outline_color);
			_uvs_buffer.push_back(glyph_data.uvs[i]);
			_alpha_buffer.push_back(alpha);
		}

		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };

		for (jgl::Size_t i = 0; i < 6; i++)
		{
			_element_buffer.push_back(element_index[i] + old_len);
		}

		return (glyph_data.offset * size);
	}
}