#pragma once

#include "jgl2_includes.h"
#include "Structure/jgl2_vector.h"
#include "Structure/jgl2_shader.h"
#include "Structure/jgl2_color.h"

namespace jgl
{
	class Font
	{
	private:
		struct GlyphData
		{
			Size_t height;
			Vector2 positions[4];
			Vector2 uvs[4];
			Vector2 step = Vector2(0, 0);
			Vector2 offset = Vector2(0, 0);
		};
		struct FontData
		{
			jgl::Size_t textSize;
			jgl::Size_t outlineSize;

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
			std::vector<GlyphData> atlas;
			GLuint id;
		};

		static inline Shader* _shader = nullptr;

		static inline Buffer* _modelSpaceBuffer = nullptr;
		static inline Buffer* _modelColorBuffer = nullptr;
		static inline Buffer* _modelOutlineColorBuffer = nullptr;
		static inline Buffer* _modelUvBuffer = nullptr;
		static inline Buffer* _indexesBuffer = nullptr;
		static inline std::vector<Vector3> _modelSpaceData = std::vector<Vector3>();
		static inline std::vector<Color> _modelColorData = std::vector<Color>();
		static inline std::vector<Color> _modelOutlineColorData = std::vector<Color>();
		static inline std::vector<Vector2> _modelUvData = std::vector<Vector2>();
		static inline std::vector<UInt> _indexesData = std::vector<UInt>();
		static inline Uniform* _textureUniform = nullptr;
		
		UChar* _fontBuffer;
		std::map<FontData, FontGlyphData> _fontGlyphDatas;

		Bool _isPixelOnGlyphOutline(UChar* p_atlasData, Int p_width, Int p_height, Int p_x, Int p_y, jgl::Int p_outlineSize);

		void _loadFontTexture(Font::FontData p_fontData);
		FontGlyphData& _getFontGlyphData(Size_t p_textSize, Size_t p_outlineSize);
		GlyphData& _getData(Size_t p_textSize, Size_t p_outlineSize, WChar p_char);
		GlyphData& _getData(FontGlyphData& p_fontGlyphData, WChar p_char);

		void _initCharRender();
		Vector2Int _prepareCharRender(FontGlyphData& p_fontGlyphData, WChar p_char, Vector2Int p_pos, UInt p_size, Color p_color, Size_t p_outlineSize, Color p_outlineColor, jgl::Float p_depth);
		void _castCharRender(GLuint p_id);

		void _initShaderData();

		Vector2 _calcCharSize(FontGlyphData& p_fontGlyphData, WChar p_char);

	public:
		Font(std::string path);
		~Font();

		GLuint textureID(Size_t p_textSize, Size_t p_outlineSize);

		Vector2Int calcCharSize(WChar p_char, UInt p_size, Size_t p_outlineSize = 0);
		Vector2Int calcStringSize(std::string p_string, UInt p_size);

		Vector2Int draw(WChar p_char, Vector2Int p_pos, UInt p_size, Color p_color, jgl::Float p_depth);
		Vector2Int draw(std::string p_string, Vector2Int p_pos, UInt p_size, Color p_color, jgl::Float p_depth);
		Vector2Int draw(WChar p_char, Vector2Int p_pos, UInt p_size, Color p_color, Size_t p_outlineSize, Color p_outlineColor, jgl::Float p_depth);
		Vector2Int draw(std::string p_string, Vector2Int p_pos, UInt p_size, Color p_color, Size_t p_outlineSize, Color p_outlineColor, jgl::Float p_depth);
	};
}