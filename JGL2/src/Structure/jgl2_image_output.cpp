#include "Structure/jgl2_image_output.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ExternalLibraries/stb_image_write.h"

#include "Structure/Application/Graphical/jgl2_graphical_application.h"

namespace jgl
{
	void checkOpengl(std::string);
	ImageOutput::ImageOutput(Vector2Int p_size)
	{
		_size = p_size;

		glGenTextures(1, &_outputTexture);
		glBindTexture(GL_TEXTURE_2D, _outputTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glGenRenderbuffers(1, &_depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, _size.x, _size.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glGenFramebuffers(1, &_frameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObject);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _outputTexture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);

		desassociate();
	}

	ImageOutput::ImageOutput(UInt p_width, UInt p_heigth) : ImageOutput(Vector2UInt(p_width, p_heigth))
	{

	}

	ImageOutput::~ImageOutput()
	{
		glDeleteFramebuffers(1, &_frameBufferObject);
		glDeleteTextures(1, &_outputTexture);
		glDeleteRenderbuffers(1, &_depthBuffer);
	}

	void ImageOutput::associate()
	{
		jgl::Application::Graphical::instance()->_window.setViewport(jgl::Vector2Int(_size.x, _size.y));
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObject);
		clear();
	}

	void ImageOutput::clear()
	{
		glViewport(0, 0, _size.x, _size.y);
		glScissor(0, 0, _size.x, _size.y);
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void ImageOutput::desassociate()
	{
		jgl::Application::Graphical::instance()->_window.reset();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Image* ImageOutput::save()
	{
		return (new jgl::Image(saveToOpenGLTexture()));
	}

	GLuint ImageOutput::saveToOpenGLTexture()
	{
		GLuint result;

		glGenTextures(1, &result);
		glBindTexture(GL_TEXTURE_2D, result);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glCopyImageSubData(_outputTexture, GL_TEXTURE_2D, 0, 0, 0, 0,
			result, GL_TEXTURE_2D, 0, 0, 0, 0,
			_size.x, _size.y, 1);

		return (result);
	}

	void ImageOutput::saveToFile(std::string p_path)
	{
		
	}
}