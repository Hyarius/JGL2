#pragma once

#include "jgl2_includes.h"
#include "jgl2_define.h"
#include "structure/jgl2_string.h"
#include "structure/jgl2_vector.h"
#include "structure/jgl2_matrix.h"

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
			Uint = GL_UNSIGNED_INT,
			Bool = GL_BOOL,
			Double = GL_DOUBLE
		};
	private:
		jgl::String _name;
		jgl::Int _location;
		jgl::Uint _element_vertex;
		GLuint _buffer;
		Type _data_type;
		Mode _mode;
		jgl::Size_t _size;
		jgl::Size_t _max_size;

		~Buffer()
		{
			glDeleteBuffers(1, &_buffer);
		}

		static jgl::String str(Mode p_type)
		{
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
		static jgl::String str(Type p_type)
		{
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
			case Type::Uint:
			{
				return ("Uint");
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

	public:
		/*
			Buffer constructor

			Optional :
			jgl::String p_name
			jgl::Int p_location
			jgl::Uint p_element_vertex
			Type p_data_type
			Mode p_mode
		*/
		Buffer(jgl::String p_name = "undefined", jgl::Int p_location = -1, jgl::Uint p_element_vertex = 3, Type p_data_type = Type::Float, Mode p_mode = Mode::Array)
		{
			_element_vertex = p_element_vertex;
			_name = p_name;
			_location = p_location;
			_data_type = p_data_type;
			glGenBuffers(1, &_buffer);
			_size = 0;
			_max_size = 0;
			_mode = p_mode;
		}

		const jgl::Size_t size() { return (_size); }
		const GLint buffer() { return (_buffer); }

		void activate()
		{
			glBindBuffer(static_cast<GLenum>(_mode), _buffer);
			if (_mode == Mode::Array)
			{
				glEnableVertexAttribArray(_location);
				glVertexAttribPointer(_location, _element_vertex, static_cast<GLenum>(_data_type), GL_FALSE, 0, NULL);
			}
		}

		template<typename T>
		void send(T* data_array, jgl::Size_t nb)
		{
			_size = sizeof(T) * nb;
			glBindBuffer(static_cast<GLenum>(_mode), _buffer);

			if (_max_size < _size)
			{
				glBufferData(static_cast<GLenum>(_mode), _size, data_array, GL_DYNAMIC_DRAW);
				_max_size = _size;
			}
			else
			{
				glBufferSubData(static_cast<GLenum>(_mode), 0, _size, data_array);
			}
		}

		template<typename T>
		jgl::Array<T> get()
		{
			T* data_array = new T[_size / sizeof(T)];
			jgl::Array<T> result;

			glGetNamedBufferSubData(_buffer, 0, _size, data_array);

			for (size_t i = 0; i < _size / sizeof(T); i++)
				result.push_back(data_array[i]);

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
			Uint,
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

	private:
		jgl::String _name;
		jgl::Int _location;
		Mode _mode;
		Type _type;
		Size _size;
		~Uniform()
		{

		}

		static jgl::String str(Mode value)
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

		static jgl::String str(Type p_type, Size p_size)
		{
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
			case Type::Uint:
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

		void _uniform_checker(Mode expected_mode, Type expected_type, Size expected_size)
		{
			if (_mode == Mode::Error || _type == Type::Error || _size == Size::Error)
				THROW_EXCEPTION(jgl::Error_level::Error, 0, "Trying to use uniform [" + _name + "] who isn't parsed correctly");
			if (_mode != expected_mode)
				THROW_EXCEPTION(jgl::Error_level::Error, 0, "Trying to send an " + str(expected_mode) + " into [" + _name + "] who is setup as an " + str(_mode) + " uniform");
			if (_type != expected_type || _size != expected_size)
				THROW_EXCEPTION(jgl::Error_level::Error, 0, "Trying to send a " + str(expected_type, expected_size) + " to uniform [" + _name + "] who is setup as " + str(_type, _size) + " uniform");
		}

	public:
		Uniform(jgl::String p_name = "undefined", jgl::Int p_location = -1, Mode p_mode = Mode::Element, Type p_type = Type::Float, Size p_size = Size::One) :
			_name(p_name),
			_location(p_location),
			_mode(p_mode),
			_type(p_type),
			_size(p_size)
		{

		}

		Mode mode() { return (_mode); }
		Type type() { return (_type); }
		Size size() { return (_size); }

		// Element part
		template <typename T, typename std::enable_if < std::is_same <jgl::Float, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Float, Size::One);
			glUniform1f(_location, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Int, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Int, Size::One);
			glUniform1i(_location, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Uint, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Uint, Size::One);
			glUniform1ui(_location, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector2, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Float, Size::Two);
			glUniform2f(_location, data.x, data.y);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector2Int, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Int, Size::Two);
			glUniform2i(_location, data.x, data.y);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector2Uint, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Uint, Size::Two);
			glUniform2ui(_location, data.x, data.y);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector3, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Float, Size::Three);
			glUniform3f(_location, data.x, data.y, data.z);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector3Int, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Int, Size::Three);
			glUniform3i(_location, data.x, data.y, data.z);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector3Uint, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Uint, Size::Three);
			glUniform3ui(_location, data.x, data.y, data.z);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector4, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Float, Size::Four);
			glUniform4f(_location, data.x, data.y, data.z, data.w);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector4Int, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Int, Size::Four);
			glUniform4i(_location, data.x, data.y, data.z, data.w);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector4Uint, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Uint, Size::Four);
			glUniform4ui(_location, data.x, data.y, data.z, data.w);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Matrix2x2, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Matrix, Size::Two);
			glUniformMatrix2fv(_location, 1, GL_FALSE, &(data));
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Matrix3x3, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Matrix, Size::Three);
			glUniformMatrix3fv(_location, 1, GL_FALSE, &(data));
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Matrix4x4, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			_uniform_checker(Mode::Element, Type::Matrix, Size::Four);
			glUniformMatrix4fv(_location, 1, GL_FALSE, &(data));
		}

		// Array part
		template <typename T, typename std::enable_if < std::is_same <jgl::Float, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Float, Size::One);
			glUniform1fv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Int, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Int, Size::One);
			glUniform1iv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Uint, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Uint, Size::One);
			glUniform1uiv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector2, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Float, Size::Two);
			glUniform2fv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector2Int, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Int, Size::Two);
			glUniform2iv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector2Uint, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Uint, Size::Two);
			glUniform2uiv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector3, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Float, Size::Three);
			glUniform3fv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector3Int, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Int, Size::Three);
			glUniform3iv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector3Uint, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Uint, Size::Three);
			glUniform3uiv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector4, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Float, Size::Four);
			glUniform4fv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector4Int, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Int, Size::Four);
			glUniform4iv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Vector4Uint, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Uint, Size::Four);
			glUniform4uiv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Matrix2x2, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Matrix, Size::Two);
			glUniformMatrix2fv(_location, nb_element, GL_FALSE, &(data));
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Matrix3x3, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)	
		{
			_uniform_checker(Mode::Array, Type::Matrix, Size::Three);
			glUniformMatrix3fv(_location, nb_element, GL_FALSE, &(data));
		}
		template <typename T, typename std::enable_if < std::is_same <jgl::Matrix4x4, T>::value == true > ::type* = nullptr >
		void send(T* data, jgl::Size_t nb_element)
		{
			_uniform_checker(Mode::Array, Type::Matrix, Size::Four);
			glUniformMatrix4fv(_location, nb_element, GL_FALSE, &(data));
		}
	};
}