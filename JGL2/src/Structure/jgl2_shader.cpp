#include "Structure/jgl2_shader.h"

namespace jgl
{
	Shader::Shader(std::fstream& p_vertexShaderFile, std::fstream& p_fragmentShaderFile)
	{
		_initialize();

		std::stringstream vShaderStream, fShaderStream;

		try
		{
			vShaderStream << p_vertexShaderFile.rdbuf();
			fShaderStream << p_fragmentShaderFile.rdbuf();

			p_vertexShaderFile.close();
			p_fragmentShaderFile.close();
		}
		catch (...)
		{
			throw Exception(1, "Error while reading files content");
		}

		_compile(vShaderStream.str(), fShaderStream.str());
	}

	Shader::Shader(std::string p_vertexShaderCode, std::string p_fragmentShaderCode)
	{
		_initialize();

		_compile(p_vertexShaderCode, p_fragmentShaderCode);
	}

	void Shader::activate()
	{
		glUseProgram(_program);

		glBindVertexArray(_idArray);
	}

	void Shader::_initialize()
	{
		_program = glCreateProgram();
		glGenVertexArrays(1, &_idArray);
		glBindVertexArray(_idArray);
	}

	void Shader::_compileShader(UInt p_shaderIndex, std::string p_shaderCode)
	{
		Int result;
		Int len;
		const char* content = p_shaderCode.c_str();

		result = GL_FALSE;

		glShaderSource(p_shaderIndex, 1, &content, NULL);
		glCompileShader(p_shaderIndex);
		glGetShaderiv(p_shaderIndex, GL_COMPILE_STATUS, &result);

		if (result != GL_TRUE)
		{
			glGetShaderiv(p_shaderIndex, GL_INFO_LOG_LENGTH, &len);
			Char* errorMsg = new Char[len + 1];
			glGetShaderInfoLog(p_shaderIndex, len, NULL, errorMsg);
			cout << errorMsg << endl;
			throw Exception(1, errorMsg);
		}
	}

	void Shader::_compileProgram(UInt p_programID, UInt p_vertexID, UInt p_fragmentID)
	{
		Int result;

		result = GL_FALSE;
		glAttachShader(p_programID, p_vertexID);
		glAttachShader(p_programID, p_fragmentID);
		glLinkProgram(p_programID);
		glGetProgramiv(p_programID, GL_LINK_STATUS, &result);
		if (result != GL_TRUE)
		{
			throw Exception(-3, "Error while linking a program");
		}
		glDetachShader(p_programID, p_vertexID);
		glDetachShader(p_programID, p_fragmentID);
		glDeleteShader(p_vertexID);
		glDeleteShader(p_fragmentID);
	}

	void Shader::_compile(std::string p_vertexShaderCode, std::string p_fragmentShaderCode)
	{
		UInt _vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
		UInt _fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

		_compileShader(_vertex_shader_id, p_vertexShaderCode);
		_compileShader(_fragment_shader_id, p_fragmentShaderCode);
		_compileProgram(_program, _vertex_shader_id, _fragment_shader_id);
	}
}