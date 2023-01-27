#include "Structure/jgl2_font.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "ExternalLibraries/stb_truetype.h"
#include "Structure/jgl2_graphical_application.h"

namespace jgl
{
	void Font::_initShaderData()
	{
		if (_shader == nullptr)
			_shader = GraphicalApplication::instance()->shader("TextTexture2D");

		if (_modelSpaceBuffer == nullptr)
			_modelSpaceBuffer = _shader->buffer("model_space");
		if (_modelUvBuffer == nullptr)
			_modelUvBuffer = _shader->buffer("model_uvs");
		if (_indexesBuffer == nullptr)
			_indexesBuffer = _shader->elementBuffer();
		if (_textureUniform == nullptr)
			_textureUniform = _shader->uniform("textureID");
		if (_textColorUniform == nullptr)
			_textColorUniform = _shader->uniform("textColor");
		if (_outlineColorUniform == nullptr)
			_outlineColorUniform = _shader->uniform("outlineColor");

		if (_shader == nullptr)
			throw std::runtime_error("Error : no shader TextTexture2D in application");

		if (_modelSpaceBuffer == nullptr)
			throw std::runtime_error("Error : no model space buffer found in shader");
		if (_modelUvBuffer == nullptr)
			throw std::runtime_error("Error : no model uvs buffer found in shader");
		if (_indexesBuffer == nullptr)
			throw std::runtime_error("Error : no element buffer found in shader");
		if (_textureUniform == nullptr)
			throw std::runtime_error("Error : no texture ID uniform found in shader");
		if (_textColorUniform == nullptr)
			throw std::runtime_error("Error : no text color uniform found in shader");
		if (_outlineColorUniform == nullptr)
			throw std::runtime_error("Error : no outline color uniform found in shader");
	}

	Bool Font::_isPixelOnGlyphOutline(UChar* p_atlasData, Int p_width, Int p_height, Int p_x, Int p_y, Int p_outlineSize)
	{
		Vector2Int origin = Vector2Int(p_x, p_y);

		for (jgl::Int i = p_x - p_outlineSize; i <= p_x + p_outlineSize; i++)
		{
			for (jgl::Int j = p_y - p_outlineSize; j <= p_y + p_outlineSize; j++)
			{
				if (i >= 0 && j >= 0 &&
					i < p_width && j < p_height)
				{
					Size_t index = i + j * p_width;
					if (p_atlasData[index] == 0x00)
						return (true);
				}
			}
		}

		return (false);
	}

	Font::Font(std::string p_path)
	{
		Long size;

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

		_fontBuffer = (UChar*)malloc(size * sizeof(UChar));

		fread(_fontBuffer, size, 1, fontFile);
		fclose(fontFile);
	}

	Font::~Font()
	{
		//glDeleteTextures(1, &_id);
	}

	Size_t Font::computeTextSize(std::string p_string, Vector2Int p_textArea)
	{
		std::vector<int> deltas = { 100, 50, 20, 10, 1 };
		Size_t result = 2;

		if (p_string == "")
			return (p_textArea.y());

		for (int i = 0; i < deltas.size(); i++)
		{
			bool enough = false;
			while (enough == false)
			{
				Vector2Int tmp_size = calcStringSize(p_string, result + deltas[i]);
				if (tmp_size.x() >= p_textArea.x() || tmp_size.y() >= p_textArea.y())
					enough = true;
				else
					result += deltas[i];
			}
		}
		return (result);
	}

	GLuint Font::textureID(Size_t p_textSize, Size_t p_outlineSize)
	{
		auto tmp = getFontGlyphData(p_textSize, p_outlineSize);
		return (tmp.id);
	}

	void Font::_loadFontTexture(Font::FontData p_fontData)
	{
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
			if (!stbtt_PackFontRange(&context, _fontBuffer, 0, p_fontData.textSize, 0, nb_char, char_info)) {
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

		FontGlyphData tmp_fontGlyphData;

		tmp_fontGlyphData.size = jgl::Vector2Int(width, height);
		Float width_delta = 0.5f / width;
		Float height_delta = 0.5f / height;

		for (Size_t i = 32; i < nb_char; i++)
		{
			UChar c = i;
			GlyphData data;

			stbtt_aligned_quad quad;

			stbtt_GetPackedQuad(char_info, width, height, c, &data.step.x(), &data.step.y(), &quad, 1);

			const Float xmin = quad.x0;
			const Float xmax = quad.x1;
			const Float ymin = quad.y0;
			const Float ymax = quad.y1;

			data.positions[0] = Vector2(xmin, ymin);
			data.positions[1] = Vector2(xmax, ymin);
			data.positions[2] = Vector2(xmin, ymax);
			data.positions[3] = Vector2(xmax, ymax);
			data.size = Vector2Int(xmax - xmin, ymax - ymin);
			data.offset = Vector2(xmin, -ymax);

			data.uvs[0] = { quad.s0 - width_delta, quad.t0 - height_delta };
			data.uvs[1] = { quad.s1 + width_delta, quad.t0 - height_delta };
			data.uvs[2] = { quad.s0 - width_delta, quad.t1 + height_delta };
			data.uvs[3] = { quad.s1 + width_delta, quad.t1 + height_delta };

			if (tmp_fontGlyphData.atlas.size() <= c)
				tmp_fontGlyphData.atlas.resize(c + 1);
			tmp_fontGlyphData.atlas[c] = data;
		}

		for (Int y = 0; y < height; y++)
		{
			for (Int x = 0; x < width; x++)
			{
				Size_t index = x + y * width;
				if (atlasData[index] != 0x00)
				{
					if (_isPixelOnGlyphOutline(atlasData, width, height, x, y, p_fontData.outlineSize) == true)
					{
						atlasData[index] = 0x7D;
					}
					else
					{
						atlasData[index] = 0xFF;
					}
				}
			}
		}

		glGenTextures(1, &tmp_fontGlyphData.id);
		glBindTexture(GL_TEXTURE_2D, tmp_fontGlyphData.id);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, atlasData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


		_fontGlyphDatas[p_fontData] = tmp_fontGlyphData;

		delete[] atlasData;
	}

	Font::FontGlyphData& Font::getFontGlyphData(Size_t p_textSize, Size_t p_outlineSize)
	{
		Font::FontData tmp(p_textSize, p_outlineSize);

		if (_fontGlyphDatas.count(tmp) == 0)
		{
			_loadFontTexture(tmp);
		}

		return (_fontGlyphDatas[tmp]);
	}

	Font::GlyphData& Font::getData(Size_t p_textSize, Size_t p_outlineSize, UChar p_char)
	{
		FontGlyphData tmp_fontGlyphData = getFontGlyphData(p_textSize, p_outlineSize);

		if (tmp_fontGlyphData.atlas.size() < p_char)
		{
			std::string errorMessage = std::string("Trying to print an unexpected char [") + static_cast<Char>(p_char) + "](" + std::to_string(static_cast<long>(p_char)) + ")";
			throw std::runtime_error(errorMessage.c_str());
		}
		return (tmp_fontGlyphData.atlas[p_char]);
	}

	Font::GlyphData& Font::getData(FontGlyphData& p_fontGlyphData, UChar p_char)
	{
		if (p_fontGlyphData.atlas.size() < p_char)
		{
			std::string errorMessage = std::string("Trying to print an unexpected char [") + static_cast<Char>(p_char) + "](" + std::to_string(static_cast<long>(p_char)) + ")";
			throw std::runtime_error(errorMessage.c_str());
		}
		return (p_fontGlyphData.atlas[p_char]);
	}

	void Font::_initCharRender()
	{
		_modelSpaceData.clear();
		_modelUvData.clear();
		_indexesData.clear();
	}

	Vector2 Font::_calcCharSize(FontGlyphData& p_fontGlyphData, UChar p_char)
	{
		Vector2Int minValue = Vector2Int(0, 0);
		Vector2Int maxValue = Vector2Int(0, 0);

		Font::GlyphData& glyphData = getData(p_fontGlyphData, static_cast<UChar>(p_char));

		maxValue.x() += glyphData.size.x();

		if (minValue.y() > glyphData.offset.y())
			minValue.y() = glyphData.offset.y();

		if (maxValue.y() < glyphData.size.y())
			maxValue.y() = glyphData.size.y();

		return (maxValue - minValue);
	}

	Vector2Int Font::calcCharSize(UChar p_char, UInt p_size, Size_t p_outlineSize)
	{
		Vector2Int minValue = Vector2Int(0, 0);
		Vector2Int maxValue = Vector2Int(0, 0);

		FontGlyphData& tmp_fontGlyphData = getFontGlyphData(p_size, 0);

		Font::GlyphData& glyphData = getData(tmp_fontGlyphData, static_cast<UChar>(p_char));

		maxValue.x() += glyphData.size.x();

		if (minValue.y() > glyphData.offset.y())
			minValue.y() = glyphData.offset.y();

		if (maxValue.y() < glyphData.size.y())
			maxValue.y() = glyphData.size.y();

		return (maxValue - minValue);
	}

	Vector2Int Font::calcStringSize(std::string p_string, UInt p_size)
	{
		Vector2Int minValue = Vector2Int(0, 0);
		Vector2Int maxValue = Vector2Int(0, 0);

		FontGlyphData& tmp_fontGlyphData = getFontGlyphData(p_size, 0);

		for (Size_t i = 0; i < p_string.size(); i++)
		{
			Font::GlyphData& glyphData = getData(tmp_fontGlyphData, static_cast<UChar>(p_string[i]));
			
			maxValue.x() += glyphData.step.x();

			if (minValue.y() > glyphData.offset.y())
				minValue.y() = glyphData.offset.y();

			if (maxValue.y() < glyphData.size.y())
				maxValue.y() = glyphData.size.y();
		}

		return (maxValue - minValue);
	}

	Vector2Int Font::_prepareCharRender(FontGlyphData& p_fontGlyphData, UChar p_char, Vector2Int p_pos, UInt p_size, Size_t p_outlineSize, Float p_depth)
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

		Font::GlyphData& glyphData = getData(p_fontGlyphData, static_cast<UChar>(p_char));
		Vector2 BaseGlyphSize = _calcCharSize(p_fontGlyphData, p_char);
		Vector2Int glyphSize = BaseGlyphSize;

		Size_t elementSize = static_cast<Size_t>(_modelSpaceData.size());
		for (Size_t i = 0; i < 4; i++)
		{
			Vector2Int tmp_pos = p_pos + glyphSize * delta_pos[i];
			Vector3 tmp_vertex = Vector3(GraphicalApplication::instance()->convertScreenToOpenGL(tmp_pos), jgl::GraphicalApplication::instance()->convertDepthToOpenGL(p_depth));
			_modelSpaceData.push_back(tmp_vertex);
			_modelUvData.push_back(glyphData.uvs[i]);
		}

		for (Size_t i = 0; i < 6; i++)
		{
			_indexesData.push_back(elementSize + element_index[i]);
		}

		return (glyphData.step);
	}


	void Font::exportShaderData(Buffer* p_modelSpaceBuffer, Buffer* p_modelUvBuffer, Buffer* p_indexesBuffer)
	{
		if (p_modelSpaceBuffer != nullptr)
			p_modelSpaceBuffer->send(_modelSpaceData.data(), static_cast<Size_t>(_modelSpaceData.size()));
		if (p_modelUvBuffer != nullptr)
			p_modelUvBuffer->send(_modelUvData.data(), static_cast<Size_t>(_modelUvData.size()));
		if (p_indexesBuffer != nullptr)
			p_indexesBuffer->send(_indexesData.data(), static_cast<Size_t>(_indexesData.size()));
	}

	void Font::_castCharRender(GLuint p_id, Color p_textColor, Color p_outlineColor)
	{
		exportShaderData(_modelSpaceBuffer, _modelUvBuffer, _indexesBuffer);

		_shader->activate();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, p_id);

		_textureUniform->send(0);
		_textColorUniform->send(p_textColor);
		_outlineColorUniform->send(p_outlineColor);

		_shader->launch(Shader::Mode::Triangle);
	}
	Vector2Int Font::draw(UChar p_char, Vector2Int p_pos, UInt p_size, Color p_color, Float p_depth)
	{
		FontGlyphData& tmp_fontGlyphData = getFontGlyphData(p_size, 0);

		Vector2Int result = prepareDraw(p_char, p_pos, p_size, 0, p_depth);

		_castCharRender(tmp_fontGlyphData.id, p_color, p_color);

		return (result);
	}
	Vector2Int Font::draw(std::string p_string, Vector2Int p_pos, UInt p_size, Color p_color, Float p_depth)
	{
		FontGlyphData& tmp_fontGlyphData = getFontGlyphData(p_size, 0);

		Vector2Int result = prepareDraw(p_string, p_pos, p_size, 0, p_depth);

		_castCharRender(tmp_fontGlyphData.id, p_color, p_color);

		return (result);
	}
	Vector2Int Font::draw(UChar p_char, Vector2Int p_pos, UInt p_size, Color p_color, Size_t p_outlineSize, Color p_outlineColor, Float p_depth)
	{
		FontGlyphData& tmp_fontGlyphData = getFontGlyphData(p_size, p_outlineSize);

		Vector2Int result = prepareDraw(p_char, p_pos, p_size, p_outlineSize, p_depth);

		_castCharRender(tmp_fontGlyphData.id, p_color, p_outlineColor);

		return (result);
	}
	Vector2Int Font::draw(std::string p_string, Vector2Int p_pos, UInt p_size, Color p_color, Size_t p_outlineSize, Color p_outlineColor, Float p_depth)
	{
		FontGlyphData& tmp_fontGlyphData = getFontGlyphData(p_size, p_outlineSize);

		Vector2Int result = prepareDraw(p_string, p_pos, p_size, p_outlineSize, p_depth);

		_castCharRender(tmp_fontGlyphData.id, p_color, p_outlineColor);

		return (result);
	}

	Vector2Int Font::prepareDraw(UChar p_char, Vector2Int p_pos, UInt p_size, Float p_depth)
	{
		_initCharRender();

		Vector2Int result;

		FontGlyphData& tmp_fontGlyphData = getFontGlyphData(p_size, 0);

		result = _prepareCharRender(tmp_fontGlyphData, p_char, p_pos, p_size, 0, p_depth);

		return (result);
	}

	Vector2Int Font::prepareDraw(std::string p_string, Vector2Int p_pos, UInt p_size, Float p_depth)
	{
		_initCharRender();

		Vector2Int result = Vector2Int(0, p_size);

		FontGlyphData& tmp_fontGlyphData = getFontGlyphData(p_size, 0);

		for (Size_t i = 0; i < p_string.size(); i++)
		{
			Vector2Int tmp = _prepareCharRender(tmp_fontGlyphData, static_cast<UChar>(p_string[i]), p_pos + result, p_size, 0, p_depth);

			result.x() += tmp.x();
		}

		return (result);
	}

	Vector2Int Font::prepareDraw(UChar p_char, Vector2Int p_pos, UInt p_size, Size_t p_outlineSize, Float p_depth)
	{
		_initCharRender();

		Vector2Int result;

		FontGlyphData& tmp_fontGlyphData = getFontGlyphData(p_size, p_outlineSize);

		result = _prepareCharRender(tmp_fontGlyphData, p_char, p_pos, p_size, p_outlineSize, p_depth);

		return (result);
	}

	Vector2Int Font::prepareDraw(std::string p_string, Vector2Int p_pos, UInt p_size, Size_t p_outlineSize, Float p_depth)
	{
		_initCharRender();

		Vector2Int result = Vector2Int(0, 0);
		Vector2Int textOffset = calcStringSize(p_string, p_size) * jgl::Vector2Int(0, 1);

		FontGlyphData& tmp_fontGlyphData = getFontGlyphData(p_size, p_outlineSize);

		for (Size_t i = 0; i < p_string.size(); i++)
		{
			Font::GlyphData& tmp_glyphData = getData(tmp_fontGlyphData, static_cast<UChar>(p_string[i]));

			jgl::Vector2Int charOffset = jgl::Vector2Int(tmp_glyphData.offset.x(), textOffset.y() - tmp_glyphData.size.y() - tmp_glyphData.offset.y());

			Vector2Int tmp = _prepareCharRender(tmp_fontGlyphData, p_string[i], p_pos + charOffset + result, p_size, p_outlineSize, p_depth);

			result.x() += tmp.x();
		}

		return (result);
	}
}