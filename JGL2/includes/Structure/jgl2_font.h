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
		struct Glyph_data
		{
			Size_t height;
			Vector2 positions[4];
			Vector2 uvs[4];
			Vector2 step = Vector2(0, 0);
			Vector2 offset = Vector2(0, 0);
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
		
		GLuint _id;
		std::vector<Glyph_data> _atlas;

		Bool _isPixelOnGlyphOutline(UChar* p_atlasData, Int p_width, Int p_height, Int p_x, Int p_y, jgl::Int p_outlineSize);

		Glyph_data& _getData(WChar p_char);

		void _initCharRender();
		Vector2Int _prepareCharRender(WChar p_char, Vector2Int p_pos, UInt p_size, Color p_color, Color p_outlineColor, jgl::Float p_depth);
		void _castCharRender();

		void _initShaderData();

	public:
		Font(std::string path, Size_t p_baseCharSize = 100, jgl::Size_t p_outlineSize = 2);
		~Font();

		Vector2Int calcCharSize(WChar p_char, UInt p_size);
		Vector2Int calcStringSize(std::string p_string, UInt p_size);

		Vector2Int draw(WChar p_char, Vector2Int p_pos, UInt p_size, Color p_color, jgl::Float p_depth);
		Vector2Int draw(std::string p_string, Vector2Int p_pos, UInt p_size, Color p_color, jgl::Float p_depth);
		Vector2Int draw(WChar p_char, Vector2Int p_pos, UInt p_size, Color p_color, Color p_outlineColor, jgl::Float p_depth);
		Vector2Int draw(std::string p_string, Vector2Int p_pos, UInt p_size, Color p_color, Color p_outlineColor, jgl::Float p_depth);
	};
}