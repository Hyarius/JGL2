#pragma once

#include "Structure/jgl2_shader.h"

namespace jgl
{
	class Image
	{
	protected:
		GLuint _id;
		unsigned char* _data;

		Int _width = 0;
		Int _height = 0;
		Int _nbChannels = 0;

		static Shader* _shader;

		static Buffer* _modelSpaceBuffer;
		static Buffer* _modelUvBuffer;
		static Buffer* _indexesBuffer;
		static Uniform* _textureUniform;

		void _init_shader_data();

	public:
		Image(GLuint p_id = -1);
		Image(std::string path);

		~Image();

		const unsigned char* data() { return reinterpret_cast<const unsigned char*>(_data); }
		const Int id() { return (_id); }
		const Int width() { return (_width); }
		const Int height() { return (_height); }

		UInt pixel(UInt p_x, UInt p_y);
		UInt pixel(Vector2UInt p_pos) { return (pixel(p_pos.x(), p_pos.y())); }

		void activate(GLenum texture = GL_TEXTURE0);
		void desactivate(GLenum texture = GL_TEXTURE0);

		void draw(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size);
	};
}