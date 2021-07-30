#include "jgl2.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external_lib/stb_image.h"

extern jgl::Array<jgl::Image*>* g_application_images_array;
jgl::Shader* base_shader = nullptr;

namespace jgl
{
	Image_handler::Image_handler()
	{
		_id = -1;
		_program = nullptr;
	}
	Image_handler::Image_handler(jgl::String path)
	{
		jgl::Int width, height, nrChannels;

		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		if (g_application_images_array == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "No application started while loading an image");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		if (base_shader == nullptr)
			base_shader = jgl::Application::active_application()->shader("Texture_shader_2D");
		if (base_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "No shader loaded for texture 2D");
		_program = base_shader;
	}
	Image_handler::Image_handler(GLuint p_id)
	{
		_id = p_id;

		if (base_shader == nullptr)
			base_shader = jgl::Application::active_application()->shader("Texture_shader_2D");
		if (base_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "No shader loaded for texture 2D");
		_program = base_shader;
	}

	Image_handler::~Image_handler()
	{
		if (_id != -1)
			glDeleteTextures(1, &_id);
	}

	Image::Image(GLuint p_id) : jgl::Image_handler(p_id)
	{
		g_application_images_array->push_back(this);
	}
	Image::Image(jgl::String path) : jgl::Image_handler(path)
	{
		g_application_images_array->push_back(this);
	}

	Image::~Image()
	{
		if (g_application_images_array == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "No application started while deleting an image");
		auto it = std::find(g_application_images_array->begin(), g_application_images_array->end(), this);
		if (it != g_application_images_array->end())
			g_application_images_array->erase(it);
	}

	void Image::draw(jgl::Vector2Int pos_a, jgl::Vector2Int pos_b, jgl::Vector2Int pos_c,
		jgl::Vector2 uv_a, jgl::Vector2 uv_b, jgl::Vector2 uv_c,
		jgl::Float depth, jgl::Float alpha, const jgl::Viewport* viewport)
	{
		_draw(jgl::convert_screen_to_opengl(pos_a, depth), jgl::convert_screen_to_opengl(pos_b, depth), jgl::convert_screen_to_opengl(pos_c, depth),
			uv_a, uv_b, uv_c,
			_element_buffer.size(), _element_buffer.size() + 1, _element_buffer.size() + 2,
			depth, alpha,
			viewport);
	}

	void Image::draw(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Vector2 uv_a, jgl::Vector2 uv_b, jgl::Vector2 uv_c, jgl::Vector2 uv_d,
		jgl::Float depth, jgl::Float alpha, const jgl::Viewport* viewport)
	{
		jgl::Size_t base_size = _vertex_buffer.size();
		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };
		static jgl::Vector2Int delta_pos[4] = {
			jgl::Vector2Int(0, 0),
			jgl::Vector2Int(1, 0),
			jgl::Vector2Int(0, 1),
			jgl::Vector2Int(1, 1)
		};

		for (size_t i = 0; i < 4; i++)
		{
			_vertex_buffer.push_back(convert_screen_to_opengl(pos + size * delta_pos[i], depth));
			_uvs_buffer.push_back(delta_pos[i]);
			_alpha_buffer.push_back(alpha);
		}

		for (size_t i = 0; i < 6; i++)
			_element_buffer.push_back(element_index[i] + base_size);
	}

	void Image::draw(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Vector2 uv_pos, jgl::Vector2 uv_size,
		jgl::Float depth, jgl::Float alpha, const jgl::Viewport* viewport)
	{
		jgl::Size_t base_size = _vertex_buffer.size();
		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };
		static jgl::Vector2Int delta_pos[4] = {
			jgl::Vector2Int(0, 0),
			jgl::Vector2Int(1, 0),
			jgl::Vector2Int(0, 1),
			jgl::Vector2Int(1, 1)
		};

		for (size_t i = 0; i < 4; i++)
		{
			_vertex_buffer.push_back(convert_screen_to_opengl(pos + size * delta_pos[i], depth));
			_uvs_buffer.push_back(uv_pos + uv_size * delta_pos[i]);
			_alpha_buffer.push_back(alpha);
		}

		for (size_t i = 0; i < 6; i++)
			_element_buffer.push_back(element_index[i] + base_size);
	}
}