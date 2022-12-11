#pragma once

#include "jgl2.h"
#include "GameEngine/Component/jgl2_component.h"
#include "GameEngine/jgl2_mesh.h"

class SpriteRenderer : public Component
{
private:
	static inline std::string C_VERTEX_SHADER_CODE = 
		R"(	#version 440 core
			layout(location = 0) in vec3 model_space;
			layout(location = 1) in vec2 model_uvs;

			uniform mat4 MVP;
	
			out vec2 fragmentUV;

			void main()
			{
				gl_Position = MVP * vec4(model_space, 1.0f);
				fragmentUV = model_uvs;
			})";

	static inline std::string C_FRAGMENT_SHADER_CODE =
		R"( #version 440 core
			in vec2 fragmentUV;

			layout(location = 0) out vec4 color;

			uniform sampler2D textureID;

			void main()
			{
				color = texture(textureID, fragmentUV).rgba;
				if (color.a == 0)
					discard;
			})";
	static inline jgl::Bool _staticInitialized = false;
	static inline jgl::Shader* _shader = nullptr;
	static inline jgl::Uniform* _matrixUniform = nullptr;
	static inline jgl::Uniform* _depthUniform = nullptr;
	static inline jgl::Uniform* _textureUniform = nullptr;

	jgl::Bool _initialized = false;
	jgl::Buffer* _modelVertexBuffer = nullptr;
	jgl::Buffer* _modelUVSBuffer = nullptr;
	jgl::Buffer* _elementBuffer = nullptr;
	jgl::Bool _baked = false;

	Mesh _mesh;

	static void _initializeStaticOpenGL();
	void _initializeOpenGL();

	void _bake();
	void _onRender();
	void _onUpdate();
public:
	SpriteRenderer(GameObject* p_owner);

	Mesh& mesh() { return (_mesh); }
	void setMesh(const Mesh& p_mesh);

	void unbake();
};