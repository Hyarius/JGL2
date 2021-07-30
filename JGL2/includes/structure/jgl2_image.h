#pragma once

#include "jgl2_define.h"
#include "jgl2_opengl.h"

namespace jgl
{
	class Image_handler
	{
	protected:
		friend class jgl::Application;

		GLuint _id;
		Shader * _program;

		jgl::Array<jgl::Vector3> _vertex_buffer;
		jgl::Array<jgl::Vector2> _uvs_buffer;
		jgl::Array<jgl::Float> _alpha_buffer;
		jgl::Array<jgl::Uint> _element_buffer;

		void _draw(
			jgl::Vector3 pos_a, jgl::Vector3 pos_b, jgl::Vector3 pos_c,
			jgl::Vector2 uv_a, jgl::Vector2 uv_b, jgl::Vector2 uv_c,
			jgl::Uint index_a, jgl::Uint index_b, jgl::Uint index_c,
			jgl::Float depth, jgl::Float alpha,
			const jgl::Viewport* viewport)
		{
			if (_id == -1 || _program == nullptr)
				THROW_EXCEPTION(jgl::Error_level::Error, 1, "Trying to print an image non-initialized or with no program attached");

			_vertex_buffer.push_back(pos_a);
			_vertex_buffer.push_back(pos_b);
			_vertex_buffer.push_back(pos_c);

			_uvs_buffer.push_back(uv_a);
			_uvs_buffer.push_back(uv_b);
			_uvs_buffer.push_back(uv_c);

			_alpha_buffer.push_back(alpha);
			_alpha_buffer.push_back(alpha);
			_alpha_buffer.push_back(alpha);

			_element_buffer.push_back(index_a);
			_element_buffer.push_back(index_b);
			_element_buffer.push_back(index_c);
		}

		virtual void _clear()
		{
			_vertex_buffer.clear();
			_uvs_buffer.clear();
			_alpha_buffer.clear();
			_element_buffer.clear();
		}

	public:
		/*
			Default image handler constructor
		*/
		Image_handler();

		/*
			Constructor by file path
			Parse an image handler at [path] location, and send it to your GPU
		*/
		Image_handler(jgl::String path);

		/*
			Constructor by GLUint
			Create an image handler by using an image already in your GPU
		*/
		Image_handler(GLuint p_id);

		/*
			Destructor
			Will also release GPU memory allocated by this image handler
		*/
		~Image_handler();

		/*
			Returns the location inside GPU of this image handler, as jgl::Uint
		*/
		const jgl::Uint id() { return (_id); }

		/*
			Activate this image handler for rendering it on screen
		*/
		void activate(GLenum texture = GL_TEXTURE0)
		{
			glActiveTexture(texture);
			glBindTexture(GL_TEXTURE_2D, _id);
		}

		/*
			Desactivate this image handler 
		*/
		void desactivate(GLenum texture = GL_TEXTURE0)
		{
			glActiveTexture(texture);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		/*
			Returns the shader program linked to this image
		*/
		Shader* program() { return (_program); }

		/*
			Change the shader program used by this image
		*/
		void change_shader(jgl::Shader* new_shader)
		{
			_program = new_shader;
		}
	};

	class Image : public Image_handler
	{
	protected:

	public:
		/*
			Constructor by GLuint
			Create an image using an image already existing in GPU
		*/
		Image(GLuint p_id);

		/*
			Constructor from file
			Create an image by reading it from a file, and store it on GPU for rendering
		*/
		Image(jgl::String path);

		/*
			Destructor
		*/
		~Image();

		/*
			Draw a triangle at pos [pos_a] / [pos_b] / [pos_c] with UVs at [uv_a] / [uv_b] / [uv_c]
			UVs need to be counter clock wise
			[depth] represent the depth on which the image will be drawn on screen. 0 is draw behind 1, etc
			[alpha] represent the transparency of the image, express from 0 to 1. 0 mean fully transparent, 1 fully opaque
		*/
		void draw(jgl::Vector2Int pos_a, jgl::Vector2Int pos_b, jgl::Vector2Int pos_c,
			jgl::Vector2 uv_a, jgl::Vector2 uv_b, jgl::Vector2 uv_c,
			jgl::Float depth, jgl::Float alpha, const jgl::Viewport* viewport);

		/*
			Draw a rect as followed :
			Pos					Pos + size.x
					----------
					|        |
					|        |
					----------
			Pos + size.y		Pos + size

			with UVs as follow
				UV_a		  UV_b
					----------
					|        |
					|        |
					----------
				UV_c		  UV_d
			[depth] represent the depth on which the image will be drawn on screen. 0 is draw behind 1, etc
			[alpha] represent the transparency of the image, express from 0 to 1. 0 mean fully transparent, 1 fully opaque
		*/
		void draw(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Vector2 uv_a, jgl::Vector2 uv_b, jgl::Vector2 uv_c, jgl::Vector2 uv_d,
			jgl::Float depth, jgl::Float alpha, const jgl::Viewport* viewport);


		/*
			Draw a rect as followed :
			Pos					Pos + size.x
					----------
					|        |
					|        |
					----------
			Pos + size.y		Pos + size

			with UVs as follow
			uv_Pos					uv_Pos + uv_size.x
						----------
						|        |
						|        |
						----------
			uv_Pos + uv_size.y		uv_Pos + uv_size
			[depth] represent the depth on which the image will be drawn on screen. 0 is draw behind 1, etc
			[alpha] represent the transparency of the image, express from 0 to 1. 0 mean fully transparent, 1 fully opaque
		*/
		void draw(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size,
			jgl::Float depth, jgl::Float alpha, const jgl::Viewport* viewport);
	};
}