#include "jgl2.h"

namespace jgl
{
	namespace jgl
	{
		class Buffer
		{
		public:
			enum class Mode
			{
				Error = -1,
				Indexes = GL_ELEMENT_ARRAY_BUFFER,
				Array = GL_ARRAY_BUFFER
			};

			enum class Type
			{
				Error = -1,
				Float = GL_FLOAT,
				Int = GL_INT,
				UInt = GL_UNSIGNED_INT,
				Bool = GL_BOOL,
				Double = GL_DOUBLE
			};

			static std::string to_string(Mode p_type) {
				switch (p_type)
				{
				case Mode::Error:
					return ("Error");
				case Mode::Indexes:
				{
					return ("Indexes");
				}
				case Mode::Array:
				{
					return ("Array");
				}
				default:
					return ("undefined");
				}
			}

			static std::string to_string(Type p_type) {
				switch (p_type)
				{
				case Type::Error:
					return ("Error");
				case Type::Float:
				{
					return ("Float");
				}
				case Type::Int:
				{
					return ("Int");
				}
				case Type::UInt:
				{
					return ("UInt");
				}
				case Type::Bool:
				{
					return ("Bool");
				}
				case Type::Double:
				{
					return ("Double");
				}
				default:
					return ("undefined");
				}
			}

		private:
			std::string _name;
			Int _location;
			UInt _elements;
			GLuint _id;
			Type _dataType;
			Mode _mode;
			Size_t _size;
			Size_t _maxSize;

			~Buffer()
			{
				glDeleteBuffers(1, &_id);
			}

		public:
			Buffer(std::string p_name = "undefined", Int p_location = -1, UInt p_elements = 3, Type p_dataType = Type::Float, Mode p_mode = Mode::Array) :
				_name(p_name),
				_location(p_location),
				_elements(p_elements),
				_dataType(p_dataType),
				_size(0),
				_maxSize(0),
				_mode(p_mode)
			{
				glGenBuffers(1, &_id);
			}
			Buffer* copy() const {
				return (new Buffer(_name, _location, _elements, _dataType, _mode));
			}

			const std::string name() const { return (_name); }
			const Int location() const { return (_location); }
			const UInt indexes() const { return (_elements); }
			const Type data_type() const { return (_dataType); }
			const Mode mode() const { return (_mode); }

			const Size_t size() const { return (_size); }
			const GLint id() const { return (_id); }

			void activate()
			{
				if (_location == -1)
					return;

				glBindBuffer(static_cast<GLenum>(_mode), _id);
				if (_mode == Mode::Array)
				{
					glEnableVertexAttribArray(_location);
					glVertexAttribPointer(_location, _elements, static_cast<GLenum>(_dataType), GL_FALSE, 0, NULL);
				}
			}

			template<typename T>
			void send(T* data_array, Size_t nb)
			{
				if (_location == -1)
					return;

				_size = sizeof(T) * nb;
				glBindBuffer(static_cast<GLenum>(_mode), _id);

				if (_maxSize < _size)
				{
					glBufferData(static_cast<GLenum>(_mode), _size, data_array, GL_DYNAMIC_DRAW);
					_maxSize = _size;
				}
				else
				{
					glBufferSubData(static_cast<GLenum>(_mode), 0, _size, data_array);
				}
			}

			template<typename T>
			std::vector<T> get()
			{
				std::vector<T> result(_size / sizeof(T));

				glGetNamedBufferSubData(_id, 0, _size, &result[0]);

				return (result);
			}
		};

		class Uniform
		{
		public:
			enum class Mode
			{
				Error,
				Array,
				Element
			};
			enum class Type
			{
				Error,
				Float,
				Int,
				UInt,
				Matrix,
				Sampler
			};
			enum class Size
			{
				Error,
				One,
				Two,
				Three,
				Four
			};
			static std::string str(Mode value)
			{
				switch (value)
				{
				case Mode::Error:
					return ("Error");
				case Mode::Element:
					return ("Element");
				case Mode::Array:
					return ("Array");
				default:
					return ("undefined");
				}
			}

			static std::string str(Type p_type, Size p_size) {
				switch (p_type)
				{
				case Type::Error:
					return ("Error");
				case Type::Float:
				{
					switch (p_size)
					{
					case Size::Error:
						return ("Error");
					case Size::One:
						return ("Float");
					case Size::Two:
						return ("Vector2");
					case Size::Three:
						return ("Vector3");
					case Size::Four:
						return ("Vector4");
					default:
						return ("undefined");
					}
				}
				case Type::Int:
				{
					switch (p_size)
					{
					case Size::Error:
						return ("Error");
					case Size::One:
						return ("Int");
					case Size::Two:
						return ("Ivector2");
					case Size::Three:
						return ("Ivector3");
					case Size::Four:
						return ("Ivector4");
					default:
						return ("undefined");
					}
				}
				case Type::UInt:
				{
					switch (p_size)
					{
					case Size::Error:
						return ("Error");
					case Size::One:
						return ("Uint");
					case Size::Two:
						return ("Uivector2");
					case Size::Three:
						return ("Uivector3");
					case Size::Four:
						return ("Uivector4");
					default:
						return ("undefined");
					}
				}
				case Type::Matrix:
				{
					switch (p_size)
					{
					case Size::Error:
						return ("Error");
					case Size::Two:
						return ("Matrix2x2");
					case Size::Three:
						return ("Matrix3x3");
					case Size::Four:
						return ("Matrix4x4");
					default:
						return ("undefined");
					}
				}
				case Type::Sampler:
				{
					switch (p_size)
					{
					case Size::Error:
						return ("Error");
					case Size::One:
						return ("Sampler2D");
					default:
						return ("undefined");
					}
				}
				default:
					return ("undefined");
				}
			}

		private:
			std::string _name;
			Int _location;
			Mode _mode;
			Type _type;
			Size _size;

			~Uniform()
			{

			}

			bool _uniform_checker(Mode expected_mode, Type expected_type, Size expected_size)
			{
				std::string errorMessage = "";

				if (_mode == Mode::Error || _type == Type::Error || _size == Size::Error)
					errorMessage = "Trying to use uniform [" + _name + "] who isn't parsed correctly";
				if (_mode != expected_mode)
					errorMessage = "Trying to send an " + str(expected_mode) + " into [" + _name + "] who is setup as an " + str(_mode) + " uniform";
				if (_type != expected_type || _size != expected_size)
					errorMessage = "Trying to send a " + str(expected_type, expected_size) + " to uniform [" + _name + "] who is setup as " + str(_type, _size) + " uniform";
				if (_location == -1)
					errorMessage = "Trying to use uniform [" + _name + "] who isn't parsed correctly (location = -1)";

				if (errorMessage != "")
					throw Exception(1, errorMessage.c_str());

				return (true);
			}

		public:
			Uniform(std::string p_name = "undefined", Int p_location = -1, Mode p_mode = Mode::Element, Type p_type = Type::Float, Size p_size = Size::One) :
				_name(p_name),
				_location(p_location),
				_mode(p_mode),
				_type(p_type),
				_size(p_size)
			{

			}

			Uniform* copy() const {
				return (new Uniform(_name, _location, _mode, _type, _size));
			}
			const std::string name() const { return (_name); }
			const Int location() const { return (_location); }
			const Mode mode() const { return (_mode); }
			const Type type() const { return (_type); }
			const Size size() const { return (_size); }

			// Element part
			template <typename T, typename std::enable_if < std::is_same <Float, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::Float, Size::One) == true)
					glUniform1f(_location, data);
			}
			template <typename T, typename std::enable_if < std::is_same <Int, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::Int, Size::One) == true)
					glUniform1i(_location, data);
			}
			template <typename T, typename std::enable_if < std::is_same <UInt, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::UInt, Size::One) == true)
					glUniform1ui(_location, data);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector2, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::Float, Size::Two) == true)
					glUniform2f(_location, data.x, data.y);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector2Int, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::Int, Size::Two) == true)
					glUniform2i(_location, data.x, data.y);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector2UInt, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::UInt, Size::Two) == true)
					glUniform2ui(_location, data.x, data.y);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector3, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::Float, Size::Three) == true)
					glUniform3f(_location, data.x, data.y, data.z);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector3Int, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::Int, Size::Three) == true)
					glUniform3i(_location, data.x, data.y, data.z);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector3UInt, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::UInt, Size::Three) == true)
					glUniform3ui(_location, data.x, data.y, data.z);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector4, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::Float, Size::Four) == true)
					glUniform4f(_location, data.x, data.y, data.z, data.w);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector4Int, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::Int, Size::Four) == true)
					glUniform4i(_location, data.x, data.y, data.z, data.w);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector4UInt, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::UInt, Size::Four) == true)
					glUniform4ui(_location, data.x, data.y, data.z, data.w);
			}
			/*
			template <typename T, typename std::enable_if < std::is_same <Matrix2x2, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::Matrix, Size::Two) == true)
					glUniformMatrix2fv(_location, 1, GL_FALSE, &(data));
			}
			template <typename T, typename std::enable_if < std::is_same <Matrix3x3, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::Matrix, Size::Three) == true)
					glUniformMatrix3fv(_location, 1, GL_FALSE, &(data));
			}
			template <typename T, typename std::enable_if < std::is_same <Matrix4x4, T>::value == true > ::type* = nullptr >
			void send(T data)
			{
				if (_uniform_checker(Mode::Element, Type::Matrix, Size::Four) == true)
					glUniformMatrix4fv(_location, 1, GL_FALSE, &(data));
			}
			*/

			// Array part
			template <typename T, typename std::enable_if < std::is_same <Float, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::Float, Size::One) == true)
					glUniform1fv(_location, nb_element, data);
			}
			template <typename T, typename std::enable_if < std::is_same <Int, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::Int, Size::One) == true)
					glUniform1iv(_location, nb_element, data);
			}
			template <typename T, typename std::enable_if < std::is_same <UInt, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::UInt, Size::One) == true)
					glUniform1uiv(_location, nb_element, data);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector2, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::Float, Size::Two) == true)
					glUniform2fv(_location, nb_element, data);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector2Int, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::Int, Size::Two) == true)
					glUniform2iv(_location, nb_element, data);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector2UInt, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::UInt, Size::Two) == true)
					glUniform2uiv(_location, nb_element, data);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector3, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::Float, Size::Three) == true)
					glUniform3fv(_location, nb_element, data);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector3Int, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::Int, Size::Three) == true)
					glUniform3iv(_location, nb_element, data);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector3UInt, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::UInt, Size::Three) == true)
					glUniform3uiv(_location, nb_element, data);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector4, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::Float, Size::Four) == true)
					glUniform4fv(_location, nb_element, data);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector4Int, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::Int, Size::Four) == true)
					glUniform4iv(_location, nb_element, data);
			}
			template <typename T, typename std::enable_if < std::is_same <Vector4UInt, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::UInt, Size::Four) == true)
					glUniform4uiv(_location, nb_element, data);
			}
			/*
			template <typename T, typename std::enable_if < std::is_same <Matrix2x2, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::Matrix, Size::Two) == true)
					glUniformMatrix2fv(_location, nb_element, GL_FALSE, &(data));
			}
			template <typename T, typename std::enable_if < std::is_same <Matrix3x3, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::Matrix, Size::Three) == true)
					glUniformMatrix3fv(_location, nb_element, GL_FALSE, &(data));
			}
			template <typename T, typename std::enable_if < std::is_same <Matrix4x4, T>::value == true > ::type* = nullptr >
			void send(T* data, Size_t nb_element)
			{
				if (_uniform_checker(Mode::Array, Type::Matrix, Size::Four) == true)
					glUniformMatrix4fv(_location, nb_element, GL_FALSE, &(data));
			}
			*/
		};
	}

	class Shader
	{
	private:
		UInt _program;
		UInt _idArray;

		void _initialize();
		void _compileShader(UInt p_shaderIndex, std::string p_shaderCode);
		void _compileProgram(UInt p_programID, UInt p_vertexID, UInt p_fragmentID);
		void _compile(std::string p_vertexShaderCode, std::string p_fragmentShaderCode);
	public:
		Shader(std::fstream& p_vertexShaderFile, std::fstream& p_fragmentShaderFile);
		Shader(std::string p_vertexShaderCode, std::string p_fragmentShaderCode);
		void activate();
	};
}

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
			Char *errorMsg = new Char[len + 1];
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

class TestWidget : public jgl::Abstract::Widget
{
private:

	jgl::Bool _onUpdate()
	{
		return (false);
	}

	void _onRender()
	{
		jgl::draw_rectangle_color(jgl::Color(255, 0, 0), jgl::Vector2Int(100, 100), jgl::Vector2Int(100, 100));
	}

public:
	TestWidget(jgl::Abstract::Widget* p_parent) : jgl::Abstract::Widget(p_parent)
	{

	}
};

std::string colorShaderVertex =
R"(#version 330 core
layout(location = 0) in vec3 model_space;
layout(location = 1) in vec4 color_space;
out vec4 fragmentColor;
void main()
{
gl_Position = vec4(model_space, 1.0f);
fragmentColor = color_space;
})";

std::string colorShaderFragment =
R"(#version 330 core
in vec4 fragmentColor;
layout(location = 0) out vec4 color;
void main()
{
	color = fragmentColor;
	if (color.a == 0)
		discard;
})";

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("JGLTester", jgl::Vector2Int(400, 400));
	jgl::cout.setEncoding("fr-FR");

	jgl::Shader tmpShader = jgl::Shader(colorShaderVertex, colorShaderFragment);

	TestWidget tmpWidget = TestWidget(nullptr);

	return (app.run());
}