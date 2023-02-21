#pragma once

#include "Structure/jgl2_shader.h"
#include "Structure/jgl2_image.h"

namespace jgl
{
	class ImageOutput
	{
	private:
		Vector2Int _size;
		GLuint _frameBufferObject;
		GLuint _outputTexture;

	public:
		ImageOutput(Vector2Int p_size);

		ImageOutput(UInt p_width, UInt p_heigth);

		~ImageOutput();

		void associate();

		void clear();

		void desassociate();

		Image* save();

		void saveToFile(std::string p_path);
	};
}