#pragma once

#include "Structure/jgl2_shader.h"
#include "Structure/jgl2_image.h"

namespace jgl
{
	class ImageOutput
	{
	private:
		Vector2UInt _size;
		GLuint _frameBuffer;
		GLuint _id;
		GLuint _depthBuffer;
		GLenum _drawBuffers[1];

	public:
		ImageOutput(Vector2UInt p_size);

		ImageOutput(UInt p_width, UInt p_heigth);

		GLuint id() const { return (_id); }

		void associate();

		void clear();

		void desassociate();

		Image* save();

		void saveToFile(std::string p_path);

		static void check_framebuffer_status(std::string msg);
	};
}