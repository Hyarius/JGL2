#include "Structure/jgl2_font.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "ExternalLibraries/stb_truetype.h"
#include "Structure/jgl2_application.h"

namespace jgl
{
	void Font::_initShaderData()
	{
		if (_shader == nullptr)
			_shader = Application::instance()->shader("TextTexture2D");

		if (_modelSpaceBuffer == nullptr)
			_modelSpaceBuffer = _shader->buffer("model_space");
		if (_modelUvBuffer == nullptr)
			_modelUvBuffer = _shader->buffer("model_uv");
		if (_modelColorBuffer == nullptr)
			_modelColorBuffer = _shader->buffer("model_color");
		if (_modelOutlineColorBuffer == nullptr)
			_modelOutlineColorBuffer = _shader->buffer("model_outline_color");
		if (_indexesBuffer == nullptr)
			_indexesBuffer = _shader->elementBuffer();
		if (_textureUniform == nullptr)
			_textureUniform = _shader->uniform("textureID");

		if (_shader == nullptr)
			throw std::runtime_error("Error : no shader TextTexture2D in application");

		if (_modelSpaceBuffer == nullptr)
			throw std::runtime_error("Error : no model space buffer found in shader");
		if (_modelUvBuffer == nullptr)
			throw std::runtime_error("Error : no model uv buffer found in shader");
		if (_modelColorBuffer == nullptr)
			throw std::runtime_error("Error : no model color buffer found in shader");
		if (_modelOutlineColorBuffer == nullptr)
			throw std::runtime_error("Error : no model outline color buffer found in shader");
		if (_indexesBuffer == nullptr)
			throw std::runtime_error("Error : no element buffer found in shader");
		if (_textureUniform == nullptr)
			throw std::runtime_error("Error : no texture ID uniform found in shader");
	}

	Bool Font::_isPixelOnGlyphOutline(UChar* p_atlasData, Int p_width, Int p_height, Int p_x, Int p_y, jgl::Int p_outlineSize)
	{
		Vector2Int origin = Vector2Int(p_x, p_y);
		for (jgl::Int delta_y = -p_outlineSize; delta_y <= p_outlineSize; delta_y++)
			for (jgl::Int delta_x = -p_outlineSize; delta_x <= p_outlineSize; delta_x++)
			{
				jgl::Int tmp_x = p_x + delta_x;
				jgl::Int tmp_y = p_y + delta_y;
				if (tmp_x >= 0 && tmp_x < p_width && tmp_y >= 0 && tmp_y < p_height)// && Vector2(tmp_x, tmp_y).distance(origin) <= p_outlineSize)
				{
					jgl::Int index = tmp_x + tmp_y * p_width;

					if (p_atlasData[index] == 0x00)
					{
						return (true);
					}
				}
			}

		return (false);
	}

	Font::Font(std::string p_path, Size_t p_baseCharSize, jgl::Size_t p_outlineSize)
	{
		Long size;
		UChar* fontBuffer;

		FILE* fontFile = NULL;

		_initShaderData();

		errno_t err = fopen_s(
			&fontFile,
			p_path.c_str(), "rb"
		);

		if (fontFile == NULL)
		{
			std::string error = std::string("Can't open file ") + p_path.c_str();
			throw std::runtime_error("Can't open file ");
		}

		fseek(fontFile, 0, SEEK_END);
		size = ftell(fontFile); /* how long is the file ? */
		fseek(fontFile, 0, SEEK_SET); /* reset */

		fontBuffer = (UChar*)malloc(size * sizeof(UChar));

		fread(fontBuffer, size, 1, fontFile);
		fclose(fontFile);

		const Size_t nb_char = 256;

		UChar* atlasData = nullptr;

		Int width = 32;
		Int height = 32;

		stbtt_packedchar* char_info = new stbtt_packedchar[nb_char];

		while (1) {
			atlasData = new UChar[width * height];

			stbtt_pack_context context;
			if (!stbtt_PackBegin(&context, atlasData, width, height, 0, 1, nullptr))
				throw std::runtime_error("Failed to initialize font");

			stbtt_PackSetOversampling(&context, 1, 1);
			if (!stbtt_PackFontRange(&context, fontBuffer, 0, p_baseCharSize, 0, nb_char, char_info)) {
				// too small
				delete[] atlasData;
				stbtt_PackEnd(&context);
				width *= 2;
				height *= 2;
			}
			else {
				stbtt_PackEnd(&context);
				break;
			}
		}

		Float width_delta = 0.5f / width;
		Float height_delta = 0.5f / height;
		for (Size_t i = 32; i < nb_char; i++)
		{
			WChar c = i;
			Glyph_data data;

			stbtt_aligned_quad quad;

			stbtt_GetPackedQuad(char_info, width, height, c, &data.step.x(), &data.step.y(), &quad, 1);

			const Float xmin = quad.x0;
			const Float xmax = quad.x1;
			const Float ymin = -quad.y1;
			const Float ymax = -quad.y0;

			data.height = p_baseCharSize;
			data.positions[0] = Vector2(xmin / p_baseCharSize, ymin / p_baseCharSize);
			data.positions[1] = Vector2(xmax / p_baseCharSize, ymin / p_baseCharSize);
			data.positions[2] = Vector2(xmin / p_baseCharSize, ymax / p_baseCharSize);
			data.positions[3] = Vector2(xmax / p_baseCharSize, ymax / p_baseCharSize);
			data.step /= Vector2(p_baseCharSize, p_baseCharSize);
			data.offset = Vector2(xmin / p_baseCharSize, ymin / p_baseCharSize);

			data.uvs[0] = { quad.s0 - width_delta, quad.t0 - height_delta };
			data.uvs[1] = { quad.s1 + width_delta, quad.t0 - height_delta };
			data.uvs[2] = { quad.s0 - width_delta, quad.t1 + height_delta };
			data.uvs[3] = { quad.s1 + width_delta, quad.t1 + height_delta };

			if (_atlas.size() <= c)
				_atlas.resize(c + 1);
			_atlas[c] = data;
		}

		for (jgl::Int y = 0; y < height; y++)
		{
			for (jgl::Int x = 0; x < width; x++)
			{
				jgl::Size_t index = x + y * width;
				if (atlasData[index] != 0x00)
				{
					if (_isPixelOnGlyphOutline(atlasData, width, height, x, y, p_outlineSize) == true)
					{
						atlasData[index + 0] = 0x7D;
					}
					else
					{
						atlasData[index + 0] = 0xFF;
					}
				}
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

		delete[] atlasData;
	}

	Font::~Font()
	{
		glDeleteTextures(1, &_id);
	}

	Font::Glyph_data& Font::_getData(WChar p_char)
	{
		if (_atlas.size() < p_char)
			throw std::runtime_error("Trying to print an unexpected char");
		return (_atlas[p_char]);
	}

	void Font::_initCharRender()
	{
		_modelSpaceData.clear();
		_modelUvData.clear();
		_modelColorData.clear();
		_modelOutlineColorData.clear();
		_indexesData.clear();
	}

	Vector2Int Font::calcCharSize(WChar p_char, UInt p_size)
	{
		Glyph_data& data = _getData(p_char);
		Vector2 tmp = (data.positions[3] - data.positions[0]);
		tmp *= Vector2(p_size, p_size);
		return (tmp);
	}

	Vector2Int Font::calcStringSize(std::string p_string, UInt p_size)
	{
		Vector2Int result = Vector2Int(0, p_size);

		for (jgl::Size_t i = 0; i < p_string.size(); i++)
		{
			Font::Glyph_data& glyphData = _getData(p_string[i]);

			result.x() += glyphData.step.x() * p_size;
		}

		return (result);
	}

	Vector2Int Font::_prepareCharRender(WChar p_char, Vector2Int p_pos, UInt p_size, Color p_color, Color p_outlineColor, jgl::Float p_depth)
	{
		if (p_char < 32)
			return (Vector2Int(0, 0));

		static UInt element_index[6] = { 0, 3, 1, 2, 3, 0 };
		static Vector2Int delta_pos[4] = {
			Vector2Int(0, 0),
			Vector2Int(1, 0),
			Vector2Int(0, 1),
			Vector2Int(1, 1)
		};
		Font::Glyph_data& glyphData = _getData(p_char);
		Vector2Int glyphSize = calcCharSize(p_char, p_size);
		Vector2Int glyphOffset = glyphData.offset * Vector2(p_size, -static_cast<Int>(p_size));

		jgl::Size_t elementSize = static_cast<Size_t>(_modelSpaceData.size());
		for (Size_t i = 0; i < 4; i++)
		{
			Vector2Int tmp_pos = p_pos + glyphSize * delta_pos[i] + Vector2Int(0, -glyphSize.y()) + glyphOffset;
			_modelSpaceData.push_back(Vector3(Application::instance()->convertScreenToOpenGL(tmp_pos), p_depth));
			_modelUvData.push_back(glyphData.uvs[i]);
			_modelColorData.push_back(p_color);
			_modelOutlineColorData.push_back(p_outlineColor);
		}

		for (Size_t i = 0; i < 6; i++)
		{
			_indexesData.push_back(elementSize + element_index[i]);
		}

		return (Vector2Int(glyphData.step.x() * p_size, glyphData.step.y() * p_size));
	}

	void Font::_castCharRender()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _id);

		_modelSpaceBuffer->send(_modelSpaceData.data(), static_cast<Size_t>(_modelSpaceData.size()));
		_modelUvBuffer->send(_modelUvData.data(), static_cast<Size_t>(_modelUvData.size()));
		_modelColorBuffer->send(_modelColorData.data(), static_cast<Size_t>(_modelColorData.size()));
		_modelOutlineColorBuffer->send(_modelOutlineColorData.data(), static_cast<Size_t>(_modelOutlineColorData.size()));
		_indexesBuffer->send(_indexesData.data(), static_cast<Size_t>(_indexesData.size()));

		_shader->uniform("textureID")->send(0);

		_shader->launch(jgl::Shader::Mode::Triangle);
	}
	Vector2Int Font::draw(WChar p_char, Vector2Int p_pos, UInt p_size, Color p_color, jgl::Float p_depth)
	{
		_initCharRender();

		Vector2Int result;

		result = _prepareCharRender(p_char, p_pos, p_size, p_color, p_color, p_depth);

		_castCharRender();

		return (result);
	}
	Vector2Int Font::draw(std::string p_string, Vector2Int p_pos, UInt p_size, Color p_color, jgl::Float p_depth)
	{
		_initCharRender();

		Vector2Int result = Vector2Int(0, p_size);

		for (jgl::Size_t i = 0; i < p_string.size(); i++)
		{
			Vector2Int tmp = _prepareCharRender(p_string[i], p_pos + result, p_size, p_color, p_color, p_depth);

			result.x() += tmp.x();
		}

		_castCharRender();

		return (result);
	}
	Vector2Int Font::draw(WChar p_char, Vector2Int p_pos, UInt p_size, Color p_color, Color p_outlineColor, jgl::Float p_depth)
	{
		_initCharRender();

		Vector2Int result;

		result = _prepareCharRender(p_char, p_pos, p_size, p_color, p_outlineColor, p_depth);

		_castCharRender();

		return (result);
	}
	Vector2Int Font::draw(std::string p_string, Vector2Int p_pos, UInt p_size, Color p_color, Color p_outlineColor, jgl::Float p_depth)
	{
		_initCharRender();

		Vector2Int result = Vector2Int(0, p_size);

		for (jgl::Size_t i = 0; i < p_string.size(); i++)
		{
			Vector2Int tmp = _prepareCharRender(p_string[i], p_pos + result, p_size, p_color, p_outlineColor, p_depth);

			result.x() += tmp.x();
		}

		_castCharRender();

		return (result);
	}
}