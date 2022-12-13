#pragma once

#include "jgl2.h"
#include "GameEngine/Component/jgl2_component.h"
#include "GameEngine/jgl2_text_mesh.h"

namespace jgl
{
	class TextRenderer : public Component
	{
	private:
		static inline std::string C_VERTEX_SHADER_CODE =
			R"(	#version 440 core
				layout(location = 0) in vec3 model_space;
				layout(location = 1) in vec2 model_uvs;

				uniform vec3 anchor;
				uniform mat4 MVP;
				uniform vec4 textColor;
				uniform vec4 outlineColor;
	
				out vec2 fragmentUV;
				out vec4 fragmentColor;
				out vec4 fragmentOutlineColor;

				void main()
				{
					gl_Position = MVP * vec4(model_space + anchor, 1.0f);
					fragmentUV = model_uvs;
					fragmentColor = textColor;
					fragmentOutlineColor = outlineColor;
				})";

		static inline std::string C_FRAGMENT_SHADER_CODE =
			R"( #version 440 core
				in vec2 fragmentUV;
				in vec4 fragmentColor;
				in vec4 fragmentOutlineColor;

				layout(location = 0) out vec4 color;

				uniform sampler2D textureID;

				void main()
				{
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
		static inline jgl::Bool _staticInitialized = false;
		static inline jgl::Shader* _shader = nullptr;
		static inline jgl::Uniform* _matrixUniform = nullptr;
		static inline jgl::Uniform* _anchorUniform = nullptr;
		static inline jgl::Uniform* _textureUniform = nullptr;
		static inline jgl::Uniform* _textColorUniform = nullptr;
		static inline jgl::Uniform* _outlineColorUniform = nullptr;

		jgl::Bool _initialized = false;
		jgl::Buffer* _modelVertexBuffer = nullptr;
		jgl::Buffer* _modelUVSBuffer = nullptr;
		jgl::Buffer* _elementBuffer = nullptr;
		jgl::Bool _baked = false;
	
		TextMesh _mesh;

		static void _initializeStaticOpenGL();
		void _initializeOpenGL();

		void _bake();
		void _onRender();
		void _onUpdate();

		jgl::Vector3 _computeTextOffset();

	public:
		TextRenderer(GameObject* p_owner);

		const TextMesh& mesh() const { return (_mesh); }
		void setMesh(const TextMesh& p_mesh);

		void unbake();
	};
}