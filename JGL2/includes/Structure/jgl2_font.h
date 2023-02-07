#pragma once

#include "jgl2_includes.h"
#include "Structure/jgl2_vector2.h"
#include "Structure/jgl2_shader.h"
#include "Structure/jgl2_color.h"

namespace jgl
{
	class Font
	{
	public:
		struct GlyphData
		{
			Vector2 positions[4];
			Vector2 uvs[4];
			Vector2 size = Vector2(0, 0);
			Vector2 step = Vector2(0, 0);
			Vector2 offset = Vector2(0, 0);
		};
		struct FontData
		{
			Size_t textSize;
			Size_t outlineSize;

			FontData(Size_t p_textSize, Size_t p_outlineSize)
			{
				textSize = p_textSize;
				outlineSize = p_outlineSize;
			}
			bool operator < (const FontData& p_other) const
			{
				if (textSize < p_other.textSize)
					return (true);
				if (textSize == p_other.textSize && outlineSize < p_other.outlineSize)
					return (true);
				return (false);
			}
		};
		struct FontGlyphData
		{
			jgl::Vector2Int size;
			std::vector<GlyphData> atlas;
			GLuint id;
		};

	private:

		static inline Shader* _shader = nullptr;

		static inline Buffer* _modelSpaceBuffer = nullptr;
		static inline Buffer* _modelUvBuffer = nullptr;
		static inline Buffer* _indexesBuffer = nullptr;
		static inline std::vector<Vector3> _modelSpaceData = std::vector<Vector3>();
		static inline std::vector<Vector2> _modelUvData = std::vector<Vector2>();
		static inline std::vector<UInt> _indexesData = std::vector<UInt>();
		static inline Uniform* _textureUniform = nullptr;
		static inline Uniform* _textColorUniform = nullptr;
		static inline Uniform* _outlineColorUniform = nullptr;
		
		UChar* _fontBuffer;
		std::map<FontData, FontGlyphData> _fontGlyphDatas;

		Bool _isPixelOnGlyphOutline(UChar* p_atlasData, Int p_width, Int p_height, Int p_x, Int p_y, Int p_outlineSize);

		void _loadFontTexture(Font::FontData p_fontData);

		void _initCharRender();
		Vector2Int _prepareCharRender(FontGlyphData& p_fontGlyphData, UChar p_char, Vector2Int p_pos, UInt p_size, Size_t p_outlineSize, Float p_depth);
		void _castCharRender(GLuint p_id, Color p_textColor, Color p_outlineColor);

		void _initShaderData();

		Vector2 _calcCharSize(FontGlyphData& p_fontGlyphData, UChar p_char);
		jgl::Vector2Int _calcLineOffset(FontGlyphData& p_fontGlyphData, std::string p_string);

	public:
		Font(std::string path);
		~Font();

		FontGlyphData& getFontGlyphData(Size_t p_textSize, Size_t p_outlineSize);
		GlyphData& getData(Size_t p_textSize, Size_t p_outlineSize, UChar p_char);
		GlyphData& getData(FontGlyphData& p_fontGlyphData, UChar p_char);

		Size_t computeTextSize(std::string p_string, Vector2Int p_textArea);

		GLuint textureID(Size_t p_textSize, Size_t p_outlineSize);

		void exportShaderData(Buffer* p_modelSpaceBuffer, Buffer* p_modelUvBuffer, Buffer* p_indexesBuffer);

		Vector2Int calcCharSize(UChar p_char, UInt p_size, Size_t p_outlineSize = 0);
		Vector2Int calcStringSize(std::string p_string, UInt p_size);

		Vector2Int draw(std::string p_string, Vector2Int p_pos, UInt p_size, Color p_color, Float p_depth);
		Vector2Int draw(std::string p_string, Vector2Int p_pos, UInt p_size, Color p_color, Size_t p_outlineSize, Color p_outlineColor, Float p_depth);

		Vector2Int prepareDraw(std::string p_string, Vector2Int p_pos, UInt p_size, Float p_depth);
		Vector2Int prepareDraw(std::string p_string, Vector2Int p_pos, UInt p_size, Size_t p_outlineSize, Float p_depth);
	};
}