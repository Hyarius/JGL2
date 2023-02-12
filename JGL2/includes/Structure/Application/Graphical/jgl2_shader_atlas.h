#pragma once

#include "jgl2_includes.h"
#include "structure/jgl2_shader.h"

namespace jgl
{
	class ShaderAtlas
	{
	private:
		std::map<std::string, Shader*> _shaders;

	public:
		ShaderAtlas()
		{

		}

		void add(std::string p_name, Shader* p_shader)
		{
			_shaders[p_name] = p_shader;
		}
		void add(std::string p_name, std::string p_vertexShaderCode, std::string p_fragmentShaderCode)
		{
			_shaders[p_name] = new jgl::Shader(p_vertexShaderCode, p_fragmentShaderCode);
		}
		void addFromFile(std::string p_name, std::string p_vertexShaderCode, std::string p_fragmentShaderCode)
		{
			std::fstream vertexFile;
			std::fstream fragmentFile;

			vertexFile.open(p_vertexShaderCode, std::ios_base::in);
			fragmentFile.open(p_fragmentShaderCode, std::ios_base::in);

			_shaders[p_name] = new jgl::Shader(vertexFile, fragmentFile);
		}

		jgl::Shader* get(std::string p_name)
		{
			if (_shaders.count(p_name) == 0)
				return (nullptr);
			return (_shaders[p_name]);
		}
	};
}