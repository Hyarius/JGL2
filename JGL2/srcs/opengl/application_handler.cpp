#include "jgl2.h"

extern jgl::String color_shader_vert;
extern jgl::String color_shader_frag;
extern jgl::String texture_shader_vert;
extern jgl::String texture_shader_frag;
extern jgl::String texture_bitmap_shader_vert;
extern jgl::String texture_bitmap_shader_frag;
extern jgl::String color_model_shader_vert;
extern jgl::String color_model_shader_frag;
extern jgl::String texture_model_shader_vert;
extern jgl::String texture_model_shader_frag;

jgl::Array<jgl::Image*>* g_application_images_array = nullptr;
jgl::Array<jgl::Font*>* g_application_font_array = nullptr;
jgl::Application* jgl::Application::_active_application = nullptr;
jgl::Map< jgl::Uint, bool> jgl::Viewport::_allocated_id;

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	if (severity == GL_DEBUG_TYPE_ERROR || severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR || severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR ||
		severity == GL_DEBUG_TYPE_ERROR_ARB || severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB || severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

namespace jgl
{
	Application::Application(const jgl::String& p_title, const jgl::Vector2Int& p_size, jgl::Color p_background)
	{
		jgl::start();

		_active_application = this;

		_title = p_title;
		_size = p_size;
		_background = p_background;
		_play = true;

		_create_window(p_title, _size.x, _size.y, 4, 2);
		THROW_INFORMATION("Window created");
		add_shader("Color_shader_2D", color_shader_vert, color_shader_frag);
		add_shader("Texture_shader_2D", texture_shader_vert, texture_shader_frag);
		add_shader("Texture_text_shader_2D", texture_bitmap_shader_vert, texture_bitmap_shader_frag);
		add_shader("Color_shader_3D", color_model_shader_vert, color_model_shader_frag);
		add_shader("Texture_shader_3D", texture_model_shader_vert, texture_model_shader_frag);

		_active_viewport = nullptr;

		// During init, enable debug output
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		glEnable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glFrontFace(GL_CCW);
		glStencilMask(0xFF);
		glStencilFunc(GL_EQUAL, 1, 0xFF);

		resize(_size.x, _size.y);

		g_application_images_array = &_images_array;
		g_application_font_array = &_font_array;

		_central_widget = new Application::Central_widget(nullptr);
		_central_widget->set_name("Central widget");
		_central_widget->set_geometry(0, _size);
		_central_widget->activate();

		set_active_application(this);
	}

	Application::~Application()
	{
		jgl::stop();
	}

	void Application::_handle_win_message()
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // If we have a message to process, process it
			if (msg.message == WM_QUIT) {
				quit(); // Set running to false if we have a message to quit
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	void Application::resize(jgl::Int x, jgl::Int y)
	{
		_size = jgl::Vector2Int(x, y);
		jgl::Vector2 old_size = _opengl_context.size();
		_opengl_context.resize(x, y);
		if (_central_widget != nullptr)
		{
			if (old_size.x != 0 && old_size.y != 0)
			{
				jgl::Vector2 ratio = jgl::Vector2(_size.x, _size.y) / jgl::Vector2(old_size.x, old_size.y);
				THROW_INFORMATION("Resizing with ratio " + ratio.text());
				_central_widget->_resize_children(ratio);
			}
			else
			{
				_central_widget->set_geometry(0, _size, 0);
			}
		}
	}

	void Application::_clear()
	{
		_central_widget->viewport()->use();
		_opengl_context.clear();
	}

	void Application::_render()
	{
		_opengl_context.render();
	}

	void Application::quit()
	{
		_play = false;
	}

	jgl::Int Application::run()
	{
		jgl::Ulong next_tick = GetTickCount();
		_opengl_context.setup(_background);

		while (_play == true)
		{
			_time = GetTickCount();

			_clear();
			_handle_win_message();


			_central_widget->_render_children();
			_central_widget->_update_children();

			if (next_tick <= _time)
			{
				_central_widget->_fixed_update_children();
				next_tick = next_tick + _tick_delta;
			}

			_launch_draw_triangle_color();
			_launch_draw_triangle_texture();
			_launch_draw_triangle_font();

			_update_input();

			_render();
		}
		return (1);
	}

	jgl::Shader* Application::add_shader_from_file(jgl::String name, jgl::String vertex_shader_path, jgl::String fragment_shader_path)
	{
		THROW_INFORMATION("Creating shader [" + name + "] from file");
		_shader_map[name] = jgl::Shader::compile_from_file(vertex_shader_path, fragment_shader_path);
		return (_shader_map[name]);
	}

	jgl::Shader* Application::add_shader(jgl::String name, jgl::String vertex_content, jgl::String fragment_content)
	{
		THROW_INFORMATION("Creating shader [" + name + "] from source");
		_shader_map[name] = jgl::Shader::compile(vertex_content, fragment_content);
		return (_shader_map[name]);
	}

	extern jgl::Array<jgl::Vector3> _vertex_component_color_2D_shader_data;
	extern jgl::Array<jgl::Color> _color_component_color_2D_shader_data;
	extern jgl::Array<jgl::Uint> _element_index_array_color_2D_shader_data;

	void Application::_launch_draw_triangle_color()
	{
		if (_element_index_array_color_2D_shader_data.size() == 0 || _color_component_color_2D_shader_data.size() == 0 || _vertex_component_color_2D_shader_data.size() == 0)
			return;

		const jgl::String shader_name = "Color_shader_2D";

		const jgl::Vector3* tmp_vertex = _vertex_component_color_2D_shader_data.all();
		const jgl::Color* tmp_color = _color_component_color_2D_shader_data.all();
		const jgl::Uint* tmp_index = _element_index_array_color_2D_shader_data.all();

		jgl::Shader* tmp_shader = shader(shader_name);

		if (tmp_shader == nullptr)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "No shader named " + shader_name);

		tmp_shader->buffer("model_space")->send(tmp_vertex, _vertex_component_color_2D_shader_data.size());
		tmp_shader->buffer("color_space")->send(tmp_color, _color_component_color_2D_shader_data.size());
		tmp_shader->indexes_buffer()->send(tmp_index, _element_index_array_color_2D_shader_data.size());
		tmp_shader->launch(jgl::Shader::Mode::Triangle);

		_vertex_component_color_2D_shader_data.clear();
		_color_component_color_2D_shader_data.clear();
		_element_index_array_color_2D_shader_data.clear();
	}

	void Application::_launch_draw_triangle_texture()
	{
		for (jgl::Size_t i = 0; i < _images_array.size(); i++)
		{
			jgl::Shader* tmp_shader = _images_array[i]->program();
			_images_array[i]->activate();

			jgl::Image* tmp_image = _images_array[i];

			const jgl::Vector3* vertex_buffer = tmp_image->_vertex_buffer.all();
			const jgl::Vector2* uvs_buffer = tmp_image->_uvs_buffer.all();
			const jgl::Float* alpha_buffer = tmp_image->_alpha_buffer.all();
			const jgl::Uint* element_buffer = tmp_image->_element_buffer.all();

			tmp_shader->buffer("model_space")->send(vertex_buffer, tmp_image->_vertex_buffer.size());
			tmp_shader->buffer("vertexUV")->send(uvs_buffer, tmp_image->_uvs_buffer.size());
			tmp_shader->buffer("alpha_value")->send(alpha_buffer, tmp_image->_alpha_buffer.size());
			tmp_shader->indexes_buffer()->send(element_buffer, tmp_image->_element_buffer.size());
			tmp_shader->uniform("textureID")->send(tmp_image->id());

			tmp_shader->launch(jgl::Shader::Mode::Triangle);

			_images_array[i]->_clear();

			_images_array[i]->desactivate();
		}
	}

	void Application::_launch_draw_triangle_font()
	{
		for (jgl::Size_t i = 0; i < _font_array.size(); i++)
		{
			_font_array[i]->activate();

			jgl::Font* tmp_bitmap = static_cast<jgl::Font*>(_font_array[i]);
			jgl::Shader* tmp_shader = tmp_bitmap->program();

			const jgl::Vector3* vertex_buffer = tmp_bitmap->_vertex_buffer.all();
			const jgl::Vector2* uvs_buffer = tmp_bitmap->_uvs_buffer.all();
			const jgl::Color* color_buffer = tmp_bitmap->_color_buffer.all();
			const jgl::Color* color_outline_buffer = tmp_bitmap->_color_outline_buffer.all();
			const jgl::Float* alpha_buffer = tmp_bitmap->_alpha_buffer.all();
			const jgl::Uint* element_buffer = tmp_bitmap->_element_buffer.all();

			tmp_shader->buffer("model_space")->send(vertex_buffer, tmp_bitmap->_vertex_buffer.size());
			tmp_shader->buffer("vertexUV")->send(uvs_buffer, tmp_bitmap->_uvs_buffer.size());
			tmp_shader->buffer("color_space")->send(color_buffer, tmp_bitmap->_color_buffer.size());
			tmp_shader->buffer("color_outline_space")->send(color_outline_buffer, tmp_bitmap->_color_outline_buffer.size());
			tmp_shader->buffer("alpha_value")->send(alpha_buffer, tmp_bitmap->_alpha_buffer.size());
			tmp_shader->indexes_buffer()->send(element_buffer, tmp_bitmap->_element_buffer.size());
			tmp_shader->uniform("textureID")->send(tmp_bitmap->id());

			tmp_shader->launch(jgl::Shader::Mode::Triangle);

			_font_array[i]->_clear();

			_font_array[i]->desactivate();
		}
	}

	void jgl::Application::_update_input()
	{
		_keyboard._actualize_entry('\0');
		_mouse._update();
		_keyboard._update();
	}
}