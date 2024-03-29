#include "Structure/jgl2_graphical_application.h"

namespace jgl
{


	void GraphicalApplication::_create2DColorShader()
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
	void GraphicalApplication::_create2DTextureShader()
	{
		std::string textureShaderVertex =
			R"(	#version 330 core
				layout(location = 0) in vec3 model_space;
				layout(location = 1) in vec2 model_uvs;

				out vec2 UV;
				void main()
				{
					gl_Position = vec4(model_space, 1.0f);
					UV = model_uvs;
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
					if (color.a == 0)
						discard;
				})";

		addShader("Texture2D", new jgl::Shader(textureShaderVertex, textureShaderFragment));
	}
	void GraphicalApplication::_create2DTextTextureShader()
	{
		std::string textTextureShaderVertex =
			R"(	#version 330 core
				layout(location = 0) in vec3 model_space;
				layout(location = 1) in vec2 model_uvs;

				uniform vec4 textColor;
				uniform vec4 outlineColor;

				out vec2 fragmentUV;
				out vec4 fragmentColor;
				out vec4 fragmentOutlineColor;

				void main()
				{
					gl_Position = vec4(model_space, 1.0f);
					fragmentUV = model_uvs;
					fragmentColor = textColor;
					fragmentOutlineColor = outlineColor;
				})";

		std::string textTextureShaderFragment =
			R"( #version 330 core
				in vec2 fragmentUV;
				in vec4 fragmentColor;
				in vec4 fragmentOutlineColor;

				layout(location = 0) out vec4 color;

				uniform sampler2D textureID;

				void main()
				{
					if (fragmentUV.x < 0 || fragmentUV.x > 1 || fragmentUV.y < 0 || fragmentUV.y > 1)
						discard;
					vec4 tmp_color = texture(textureID, fragmentUV).rgba;
					if (tmp_color.r == 0.0f)
						discard;
					else
						if (tmp_color.r == 1.0f)
							color = fragmentColor;
						else
							color = fragmentOutlineColor;
					if (color.a == 0)
						discard;
				})";

		addShader("TextTexture2D", new jgl::Shader(textTextureShaderVertex, textTextureShaderFragment));
	}
}