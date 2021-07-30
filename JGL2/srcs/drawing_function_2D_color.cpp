#include "jgl2.h"

namespace jgl
{
	jgl::Array<jgl::Vector3> _vertex_component_color_2D_shader_data;
	jgl::Array<jgl::Color> _color_component_color_2D_shader_data;
	jgl::Array<jgl::Uint> _element_index_array_color_2D_shader_data;

	void draw_triangle_color(jgl::Vector2Int* points, jgl::Size_t size, jgl::Color color, jgl::Float depth, jgl::Viewport* viewport)
	{
		if (viewport != nullptr)
			viewport->use();

		jgl::Uint base_size = _vertex_component_color_2D_shader_data.size();
		for (jgl::Size_t i = 0; i < size; i++)
		{
			_vertex_component_color_2D_shader_data.push_back(convert_screen_to_opengl(points[i], depth));
			_color_component_color_2D_shader_data.push_back(color);
			_element_index_array_color_2D_shader_data.push_back(base_size + i);
		}
	}

	void draw_rectangle_color(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Color color, jgl::Float depth, jgl::Viewport* viewport)
	{
		if (viewport != nullptr)
		{
			viewport->use();
		}

		jgl::Size_t base_size = _vertex_component_color_2D_shader_data.size();
		static jgl::Uint element_index[6] = { 0, 3, 1, 2, 3, 0 };
		_vertex_component_color_2D_shader_data.push_back(convert_screen_to_opengl(pos + size * jgl::Vector2Int(0, 0), depth));
		_vertex_component_color_2D_shader_data.push_back(convert_screen_to_opengl(pos + size * jgl::Vector2Int(1, 0), depth));
		_vertex_component_color_2D_shader_data.push_back(convert_screen_to_opengl(pos + size * jgl::Vector2Int(0, 1), depth));
		_vertex_component_color_2D_shader_data.push_back(convert_screen_to_opengl(pos + size * jgl::Vector2Int(1, 1), depth));

		for (size_t i = 0; i < 4; i++)
			_color_component_color_2D_shader_data.push_back(color);

		for (size_t i = 0; i < 6; i++)	
			_element_index_array_color_2D_shader_data.push_back(element_index[i] + base_size);
	}
}