#include "Structure/jgl2_image_output.h"
#include "Structure/jgl2_graphical_application.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ExternalLibraries/stb_image_write.h"

namespace jgl
{
	ImageOutput::ImageOutput(Vector2UInt p_size)
	{
		desassociate();
		_size = p_size;

		glGenFramebuffers(1, &_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x(), _size.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _id, 0);

		glGenTextures(1, &_depthBuffer);
		glBindTexture(GL_TEXTURE_2D, _depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _size.x(), _size.y());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);

		desassociate();
	}

	ImageOutput::ImageOutput(UInt p_width, UInt p_heigth) : ImageOutput(Vector2UInt(p_width, p_heigth))
	{

	}

	void ImageOutput::associate()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		clear();
	}

	void ImageOutput::clear()
	{
		glViewport(0, 0, _size.x(), _size.y());
		glColorMask(TRUE, TRUE, TRUE, TRUE);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void ImageOutput::desassociate()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ImageOutput::check_framebuffer_status(std::string msg)
	{
		cout << msg << " - Framebuffer status : ";

		switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
		{
		case (GL_FRAMEBUFFER_UNDEFINED): {
			cout << "GL_FRAMEBUFFER_UNDEFINED"; break; }
		case (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT): {
			cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break; }
		case (GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT): {
			cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break; }
		case (GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER): {
			cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break; }
		case (GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER): {
			cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break; }
		case (GL_FRAMEBUFFER_UNSUPPORTED): {
			cout << "GL_FRAMEBUFFER_UNSUPPORTED"; break; }
		case (GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE): {
			cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break; }
		case (GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS): {
			cout << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break; }
		case (GL_FRAMEBUFFER_COMPLETE): {
			cout << "GL_FRAMEBUFFER_COMPLETE"; break; }
		}
		cout << endl;
	}

	Image* ImageOutput::save()
	{
		//Creation of the texture output
		GLuint output_texture;

		glGenTextures(1, &output_texture);
		glBindTexture(GL_TEXTURE_2D, output_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x(), _size.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//Setting du framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, output_texture, 0);

		_drawBuffers[0] = GL_COLOR_ATTACHMENT0;

		glDrawBuffers(1, _drawBuffers);

		return (new Image(_id));
	}

	void ImageOutput::saveToFile(std::string p_path)
	{
		GLsizei nrChannels = 4;
		GLsizei stride = nrChannels * _size.x();
		GLsizei bufferSize = stride * _size.y();
		std::vector<char> buffer(bufferSize);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _id);

		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glGetTextureImage(_id, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferSize, buffer.data());

		stbi_write_png(p_path.c_str(), _size.x(), _size.y(), 4, buffer.data(), 100);
	}
}