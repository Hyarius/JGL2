#include "Structure/jgl2_shader.h"
#include "jgl2_basic_functions.h"

#define DEBUG_LINE() jgl::cout << __FUNCTION__ << "::" << static_cast<jgl::ULong>(__LINE__) << jgl::endl

namespace jgl
{
	std::vector<std::vector<std::string>> element_type = {
		{"float", "int", "uint", "bool", "double"},
		{"vec2", "ivec2", "uivec2", "bvec2", "dvec2"},
		{"vec3", "ivec3", "uivec3", "bvec3", "dvec3"},
		{"vec4", "ivec4", "uivec4", "bvec4", "dvec4"},
	};
	std::vector<jgl::Buffer::Type> element_type_result = {
		jgl::Buffer::Type::Float, jgl::Buffer::Type::Int, jgl::Buffer::Type::UInt, jgl::Buffer::Type::Bool, jgl::Buffer::Type::Double
	};

	void Shader::_parse_buffer(std::string base)
	{
		std::vector<std::string> tab = splitString(base, '\n');

		glBindVertexArray(_bufferArray);
		if (tab.size() >= 2)
		{
			for (jgl::Size_t j = 1; j < tab.size(); j++)
			{
				std::vector<std::string> tab2 = splitString(tab[j], ';');
				
				for (jgl::Size_t i = 0; i < tab2.size(); i++)
				{
					auto skipLenght = tab2[i].find_first_not_of(" \t");
					if (tab2[i].size() >= 6u && (tab2[i].substr(skipLenght, 3) == "in " || tab2[i].substr(skipLenght, 6) == "layout"))
					{
						std::vector<std::string> tab = splitString(tab2[i], ' ');
						std::string name = tab.back();

						GLint location_id = glGetAttribLocation(_program, name.c_str());
						if (location_id == GL_INVALID_OPERATION)
							throw std::runtime_error("Error while getting a new attrib location");

						jgl::Size_t nb_component = 0;
						jgl::Size_t tmp_element_type = 0;
						
						for (jgl::Size_t i = 0; i < element_type.size() && nb_component == 0; i++)
						{
							for (jgl::Size_t j = 0; j < element_type[i].size() && nb_component == 0; j++)
							{
								if (tab[tab.size() - 2] == element_type[i][j])
								{
									nb_component = j;
									tmp_element_type = i + 1;
								}
							}
						}
						
						jgl::cout << "Creating new buffer named [" << name << "]" << jgl::endl;
						_buffers[name] = new Buffer(name, location_id, tmp_element_type, element_type_result[nb_component], jgl::Buffer::Mode::Array);
					}
				}
			}
		}
		_elementBuffer = new Buffer("element_buffer", 0, 1, jgl::Buffer::Type::UInt, jgl::Buffer::Mode::Indexes);
	}

	void Shader::_parse_uniform_information(GLint location, std::vector<std::string> tab)
	{
		std::string name = tab[2];
		jgl::Uniform::Mode mode = jgl::Uniform::Mode::Error;
		jgl::Uniform::Type type = jgl::Uniform::Type::Error;
		jgl::Uniform::Size size = jgl::Uniform::Size::Error;

		if (tab.back().back() == ']')
			mode = jgl::Uniform::Mode::Array;
		else
			mode = jgl::Uniform::Mode::Element;

		if (tab[1] == "int")
		{
			type = jgl::Uniform::Type::Int;
			size = jgl::Uniform::Size::One;
		}
		else if (tab[1] == "float")
		{
			type = jgl::Uniform::Type::Float;
			size = jgl::Uniform::Size::One;
		}
		else if (tab[1] == "uint")
		{
			type = jgl::Uniform::Type::UInt;
			size = jgl::Uniform::Size::One;
		}
		else if (tab[1] == "vec2")
		{
			type = jgl::Uniform::Type::Float;
			size = jgl::Uniform::Size::Two;
		}
		else if (tab[1] == "vec3")
		{
			type = jgl::Uniform::Type::Float;
			size = jgl::Uniform::Size::Three;
		}
		else if (tab[1] == "vec4")
		{
			type = jgl::Uniform::Type::Float;
			size = jgl::Uniform::Size::Four;
		}
		else if (tab[1] == "ivec2")
		{
			type = jgl::Uniform::Type::Int;
			size = jgl::Uniform::Size::Two;
		}
		else if (tab[1] == "ivec3")
		{
			type = jgl::Uniform::Type::Int;
			size = jgl::Uniform::Size::Three;
		}
		else if (tab[1] == "ivec4")
		{
			type = jgl::Uniform::Type::Int;
			size = jgl::Uniform::Size::Four;
		}
		else if (tab[1] == "uvec2")
		{
			type = jgl::Uniform::Type::UInt;
			size = jgl::Uniform::Size::Two;
		}
		else if (tab[1] == "uvec3")
		{
			type = jgl::Uniform::Type::UInt;
			size = jgl::Uniform::Size::Three;
		}
		else if (tab[1] == "uvec4")
		{
			type = jgl::Uniform::Type::UInt;
			size = jgl::Uniform::Size::Four;
		}
		else if (tab[1] == "mat2")
		{
			type = jgl::Uniform::Type::Matrix;
			size = jgl::Uniform::Size::Two;
		}
		else if (tab[1] == "mat3")
		{
			type = jgl::Uniform::Type::Matrix;
			size = jgl::Uniform::Size::Three;
		}
		else if (tab[1] == "mat4")
		{
			type = jgl::Uniform::Type::Matrix;
			size = jgl::Uniform::Size::Four;
		}
		else if (tab[1].find("sampler") != std::string::npos)
		{
			type = jgl::Uniform::Type::Int;
			size = jgl::Uniform::Size::One;
		}

		jgl::cout << "Creating new uniform named [" << name << "]" << jgl::endl;
		_uniforms[name] = new jgl::Uniform(name, location, mode, type, size);
	}

	void Shader::_parse_uniform(std::string base)
	{
		std::vector<std::string> tab = splitString(base, '\n');

		glBindVertexArray(_bufferArray);
		if (tab.size() >= 2)
		{
			for (jgl::Size_t j = 1; j < tab.size(); j++)
			{
				std::vector<std::string> tab2 = splitString(tab[j], ';');
				for (jgl::Size_t i = 0; i < tab2.size(); i++)
				{
					if (tab2[i].size() >= 8)
					{
						auto skipLenght = tab2[i].find_first_not_of(" \t");
						if (tab2[i].substr(skipLenght, 7) == "uniform")
						{
							std::vector<std::string> tab = splitString(tab2[i], ' ');
							GLint id = glGetUniformLocation(_program, tab[2].c_str());
							if (id == GL_INVALID_VALUE || id == GL_INVALID_OPERATION)
								throw std::runtime_error("Error while getting a new attrib location");

							_parse_uniform_information(id, tab);
						}
					}
				}
			}
		}
	}

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
			throw std::exception("Error while reading files content");
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

		glBindVertexArray(_bufferArray);
	}

	jgl::Uniform* Shader::uniform(std::string p_name)
	{
		if (_uniforms.count(p_name) == 0)
			throw std::exception(("No uniform named " + p_name + " in shader").c_str());
		return (_uniforms[p_name]);
	}
	jgl::Buffer* Shader::buffer(std::string p_name)
	{
		if (_buffers.count(p_name) == 0)
			throw std::exception(("No data buffer named " + p_name + " in shader").c_str());
		return (_buffers[p_name]);
	}

	jgl::Buffer* Shader::elementBuffer()
	{
		return (_elementBuffer);
	}

	void Shader::cast(jgl::Shader::Mode p_type, jgl::Size_t p_nb_elem)
	{
		if (p_nb_elem != 0)
		{
			glDrawElements(static_cast<GLenum>(p_type), static_cast<GLsizei>(p_nb_elem), GL_UNSIGNED_INT, nullptr);
		}
	}

	void Shader::castInstancied(jgl::Shader::Mode p_type, jgl::Size_t p_nb_vertex_model, jgl::Size_t p_nb_element)
	{
		if (p_nb_vertex_model != 0 && p_nb_element != 0)
		{
			glDrawElementsInstanced(static_cast<GLenum>(p_type), static_cast<GLsizei>(p_nb_vertex_model), GL_UNSIGNED_INT, nullptr, p_nb_element);
		}
	}

	void Shader::launch(jgl::Shader::Mode p_type)
	{
		activate();

		for (auto it = _buffers.begin(); it != _buffers.end(); it++)
		{
			it->second->activate();
		}

		_elementBuffer->activate();

		cast(p_type, _elementBuffer->size() / sizeof(jgl::UInt));
	}
	void Shader::launchInstancied(jgl::Shader::Mode p_type, jgl::Size_t p_nb_element)
	{
		activate();

		for (auto it = _buffers.begin(); it != _buffers.end(); it++)
		{
			it->second->activate();
		}

		_elementBuffer->activate();

		castInstancied(p_type, _elementBuffer->size() / sizeof(jgl::UInt), p_nb_element);
	}

	void Shader::_initialize()
	{
		_program = glCreateProgram();
		glGenVertexArrays(1, &_bufferArray);
		glBindVertexArray(_bufferArray);
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
			throw std::exception(errorMsg);
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
			throw std::runtime_error("Error while linking a program");
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

		_parse_uniform(p_vertexShaderCode);
		_parse_uniform(p_fragmentShaderCode);

		_parse_buffer(p_vertexShaderCode);
	}
}