#include "jgl2_drawing_functions.h"
#include "Structure/Application/Graphical/jgl2_application.h"

namespace jgl
{
	static jgl::UInt element_index[6] = { 0, 3, 1, 2, 3, 0 };
	static jgl::Vector2Int delta_pos[4] = {
		jgl::Vector2Int(0, 0),
		jgl::Vector2Int(1, 0),
		jgl::Vector2Int(0, 1),
		jgl::Vector2Int(1, 1)
	};

	void drawRectangleColor(jgl::Color p_color, jgl::Vector2Int p_pos, jgl::Vector2Int p_size, jgl::Float p_depth)
	{
		const std::string shader_name = "Color2D";
		static jgl::Shader* tmp_shader = nullptr;

		if (tmp_shader == nullptr)
			tmp_shader = jgl::Application::instance()->shaders().get(shader_name);

		if (tmp_shader == nullptr)
			throw std::runtime_error("Error : no shader Color2D in application");

		static jgl::Buffer* model_buffer = nullptr;
		static jgl::Buffer* depth_buffer = nullptr;
		static jgl::Buffer* color_buffer = nullptr;

		if (model_buffer == nullptr)
			model_buffer = tmp_shader->buffer("model_space");
		if (depth_buffer == nullptr)
			depth_buffer = tmp_shader->buffer("model_depth");
		if (color_buffer == nullptr)
			color_buffer = tmp_shader->buffer("model_color");

		Vector2 vertex_content[4];
		Float depth_content[4];
		Color color_content[4];

		for (size_t i = 0; i < 4; i++)
		{
			vertex_content[i] = jgl::Application::instance()->convertScreenToOpenGL(p_pos + p_size * delta_pos[i]);
			depth_content[i] = jgl::Application::instance()->convertDepthToOpenGL(p_depth);
			color_content[i] = p_color;
		}

		model_buffer->send(vertex_content, 4);
		depth_buffer->send(depth_content, 4);
		color_buffer->send(color_content, 4);
		tmp_shader->elementBuffer()->send(element_index, 6);

		tmp_shader->launch(jgl::Shader::Mode::Triangle);
	}
}