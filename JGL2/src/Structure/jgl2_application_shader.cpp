#include "Structure/jgl2_application.h"

namespace jgl
{


	void Application::_create2DColorShader()
	{
		std::string colorShaderVertex =
			R"( #version 330 core
			layout(location = 0) in vec3 model_space;
			layout(location = 1) in vec4 color_space;
			out vec4 fragmentColor;
			void main()
			{
				gl_Position = vec4(model_space, 1.0f);
				fragmentColor = color_space;
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
				layout(location = 1) in vec2 vertexUV;
				layout(location = 2) in float alpha_value;
				out vec2 UV;
				out float ALPHA;
				void main()
				{
					gl_Position = vec4(model_space, 1.0f);
					UV = vertexUV;
					ALPHA = alpha_value;
				})";

		std::string textureShaderFragment =
			R"( #version 330 core
				in vec2 UV;
				in float ALPHA;
				layout(location = 0) out vec4 color;
				uniform sampler2D textureID;
				void main()
				{
					if (UV.x < 0 || UV.x > 1 || UV.y < 0 || UV.y > 1)
						discard;
					color = texture(textureID, UV).rgba;
					color.a = color.a * ALPHA;
					if (color.a == 0)
						discard;
				})";

		addShader("Texture2D", new jgl::Shader(textureShaderVertex, textureShaderFragment));
	}
}