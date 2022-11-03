#include "Structure/jgl2_application.h"

namespace jgl
{


	void Application::_create2DColorShader()
	{
		std::string colorShaderVertex =
			R"( #version 330 core
			layout(location = 0) in vec3 model_space;
			layout(location = 1) in vec4 model_color;

			out vec4 fragmentColor;

			void main()
			{
				gl_Position = vec4(model_space, 1.0f);
				fragmentColor = model_color;
			})";

		std::string colorShaderFragment =
			R"( #version 330 core

			in vec4 fragmentColor;

			layout(location = 0) out vec4 color;

			void main()
			{
				color = fragmentColor;
				if (color.a == 0)
					discard;
			})";

		addShader("Color2D", new jgl::Shader(colorShaderVertex, colorShaderFragment));
	}
	void Application::_create2DTextureShader()
	{
		std::string textureShaderVertex =
			R"(	#version 330 core
				layout(location = 0) in vec3 model_space;
				layout(location = 1) in vec2 model_uv;

				out vec2 UV;
				void main()
				{
					gl_Position = vec4(model_space, 1.0f);
					UV = model_uv;
				})";

		std::string textureShaderFragment =
			R"( #version 330 core
				in vec2 UV;

				layout(location = 0) out vec4 color;

				uniform sampler2D textureID;

				void main()
				{
					if (UV.x < 0 || UV.x > 1 || UV.y < 0 || UV.y > 1)
						discard;
					color = texture(textureID, UV).rgba;
				})";

		addShader("Texture2D", new jgl::Shader(textureShaderVertex, textureShaderFragment));
	}
}