#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "Structure/jgl2_application.h"
#include "Structure/jgl2_shader.h"
#include "Structure/jgl2_image.h"

namespace jgl
{
	struct GameObject2D
	{
		static inline jgl::Bool _initializedStatic = false;
		static inline jgl::Shader* _shader = nullptr;

		static inline jgl::Uniform* _matrixUniform = nullptr;
		static inline jgl::Uniform* _depthUniform = nullptr;
		static inline jgl::Uniform* _textureUniform = nullptr;

		jgl::Bool _initialized = false;
		jgl::Image* _texture = nullptr;
		jgl::Buffer* _modelVertexBuffer = nullptr;
		jgl::Buffer* _modelUVSBuffer = nullptr;
		jgl::Buffer* _elementBuffer = nullptr;
		jgl::Bool _baked = false;

		static inline std::vector<jgl::Vector2> _vertexDataArray;
		static inline std::vector<jgl::Vector2> _uvsDataArray;
		static inline std::vector<jgl::UInt> _elementDataArray;

		virtual void _onBake(std::vector<jgl::Vector2>& _vertexDataArray, std::vector<jgl::Vector2>& p_uvsDataArray, std::vector<jgl::UInt>& p_elementDataArray) = 0;

		void _initOpenGLData();
		static void _initStaticOpenGLData();

		void bake();

		jgl::Bool baked() { return (_baked); }

		static void sendMVPMatrix(glm::mat4 p_MVP);
		void setTexture(jgl::Image* p_texture);
		void render(jgl::Float p_depth);
	};
}