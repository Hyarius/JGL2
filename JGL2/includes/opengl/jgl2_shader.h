#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

#include "opengl/jgl2_gl_object.h"
#include "structure/jgl2_exception.h"
#include "jgl2_file_handler.h"

namespace jgl
{
	class Shader
	{
	private:
		GLuint _program;
		GLuint _buffer_array;
		jgl::Buffer* _element_buffer;
		jgl::Map<jgl::String, jgl::Uniform*> _uniforms;
		jgl::Map<jgl::String, jgl::Buffer*> _buffers;

	public:
		enum class Mode
		{
			Point = GL_POINTS,
			Line = GL_LINES,
			Triangle = GL_TRIANGLES,
		};
		static Shader* compile(jgl::String vertex_content, jgl::String fragment_content);
		static Shader* compile_from_file(jgl::String vertex_content_path, jgl::String fragment_content_path);

	private:
		Shader();
		void _initialize();
		void _parse_buffer(jgl::String base);
		void _parse_uniform(jgl::String base);
		void _parse_uniform_information(GLint _location, jgl::Array<jgl::String> tab);
		void _compile_shader(GLuint p_id, jgl::String p_source);
		void _compute_program(GLuint p_program_id, GLuint p_vertex_shader_id, GLuint p_fragment_shader_id);
		void _load_shaders(jgl::String vertex_content, jgl::String fragment_content);

	public:
		/*
			Active this shader program
		*/
		void activate()
		{
			glUseProgram(_program);

			glBindVertexArray(_buffer_array);
		}

		/*
			Send every buffer, data + element, actualizing them, than start the rendering process with [type] mode
		*/
		void launch(jgl::Shader::Mode type);

		/*
			Send the element buffer, actualizing it, than start the rendering process with [type] mode
		*/
		void relaunch(jgl::Shader::Mode type)
		{
			activate();
			_element_buffer->activate();
			cast(type, _element_buffer->size() / 4);
		}

		/*
			Start the rendering process, rendering [nb_element] triangle with [type] mode
		*/
		void cast(jgl::Shader::Mode type, jgl::Size_t nb_elem)
		{
			if (nb_elem != 0)
				glDrawElements(static_cast<GLenum>(type), static_cast<GLsizei>(nb_elem), GL_UNSIGNED_INT, nullptr);
		}

		/*
			Returns a pointer to the uniform named [name] of throw an exception if the uniform don't exist
		*/
		jgl::Uniform* uniform(jgl::String name)
		{
			if (_uniforms.count(name) == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, 0, "No data buffer named " + name + " in shader");
			return (_uniforms[name]);
		}

		/*
			Returns a pointer to the buffer named [name] of throw an exception if the buffer don't exist
		*/
		jgl::Buffer* buffer(jgl::String name)
		{
			if (_buffers.count(name) == 0)
				THROW_EXCEPTION(jgl::Error_level::Error, 0, "No data buffer named " + name + " in shader");
			return (_buffers[name]);
		}

		/*
			Return the element buffer, where are stored the indexes of the triangle
		*/
		jgl::Buffer* indexes_buffer() { return (_element_buffer); }
	};
}