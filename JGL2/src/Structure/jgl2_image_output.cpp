#include "Structure/jgl2_image_output.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ExternalLibraries/stb_image_write.h"

namespace jgl
{
	void checkOpengl(std::string);
	ImageOutput::ImageOutput(Vector2Int p_size)
	{
		_size = p_size;

		glGenTextures(1, &_outputTexture);
		glBindTexture(GL_TEXTURE_2D, _outputTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glGenFramebuffers(1, &_frameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObject);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _outputTexture, 0);
	}

	ImageOutput::ImageOutput(UInt p_width, UInt p_heigth) : ImageOutput(Vector2UInt(p_width, p_heigth))
	{

	}

	ImageOutput::~ImageOutput()
	{
		glDeleteFramebuffers(1, &_frameBufferObject);
	}

	void ImageOutput::associate()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObject);
		glEnable(GL_DEPTH_TEST);
	}

	void ImageOutput::clear()
	{
		glViewport(0, 0, _size.x, _size.y);
		glColorMask(TRUE, TRUE, TRUE, TRUE);
		glClearColor(1, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void ImageOutput::desassociate()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Image* ImageOutput::save()
	{
		return (new jgl::Image(_outputTexture));
	}

	void ImageOutput::saveToFile(std::string p_path)
	{
		
	}
}