#include "structure/jgl2_iostream.h"

namespace jgl
{
	thread_local jgl::IOStream cout = jgl::IOStream("Main");

	void IOStream::_addChar(jgl::Char p_char)
	{
		if (_buffer_len >= C_BUFFER_SIZE)
		{
			flush();
		}

		if (p_char == '\n')
		{
			_needed_prefix = true;
		}
		else
		{
			_buffer[_buffer_len] = p_char;
			_buffer_len++;
		}
	}
	void IOStream::_addString(std::string p_str)
	{
		for (jgl::Size_t i = 0; i < p_str.size(); i++)
		{
			_addChar(p_str[i]);
		}
	}

	IOStream::IOStream(const jgl::String& p_prefix) : _prefix(p_prefix)
	{

	}

	void IOStream::setEncoding(const char* locale, jgl::Int category)
	{
		setlocale(category, locale);
	}

	void IOStream::flush()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		if (_needed_prefix == true)
		{
			int first = _write(1, "[", 1);
			int second = _write(1, _prefix.c_str(), _prefix.size());
			int thrid = _write(1, "] - ", 4);
			_needed_prefix = false;
		}
		int forth = _write(1, _buffer, _buffer_len);
		int fifth = _write(1, "\n", 1);

		_buffer_len = 0;
	}

	IOStream& IOStream::operator << (const FormatFunctionPointer& p_funct)
	{
		return (*p_funct)(*this);
	}

	IOStream& IOStream::operator << (const jgl::Char p_char)
	{
		_addChar(p_char);
		return (*this);
	}

	IOStream& IOStream::operator << (const jgl::UChar p_char)
	{
		_addChar(p_char);
		return (*this);
	}

	IOStream& IOStream::operator << (const jgl::Char* p_str)
	{
		for (jgl::Size_t i = 0; p_str[i] != '\0'; i++)
		{
			_addChar(p_str[i]);
		}
		return (*this);
	}

	IOStream& IOStream::operator << (const jgl::Short p_value)
	{
		_addString(std::to_string(p_value));
		return (*this);
	}

	IOStream& IOStream::operator << (const jgl::UShort p_value)
	{
		_addString(std::to_string(p_value));
		return (*this);
	}

	IOStream& IOStream::operator << (const jgl::Int p_value)
	{
		_addString(std::to_string(p_value));
		return (*this);
	}

	IOStream& IOStream::operator << (const jgl::UInt p_value)
	{
		_addString(std::to_string(p_value));
		return (*this);
	}

	IOStream& IOStream::operator << (const jgl::Long p_value)
	{
		_addString(std::to_string(p_value));
		return (*this);
	}

	IOStream& IOStream::operator << (const jgl::ULong p_value)
	{
		_addString(std::to_string(p_value));
		return (*this);
	}

	IOStream& IOStream::operator << (const jgl::Float p_value)
	{
		_addString(std::to_string(p_value));
		return (*this);
	}

	IOStream& IOStream::operator << (const jgl::Double p_value)
	{
		_addString(std::to_string(p_value));
		return (*this);
	}

	jgl::IOStream& IOStream::operator << (const Glyph& p_glyph)
	{
		for (jgl::Size_t i = 0; i < p_glyph.size(); i++)
		{
			_addChar(p_glyph[i]);
		}
		return (*this);
	}

	jgl::IOStream& IOStream::operator << (const String& p_values)
	{
		for (jgl::Size_t i = 0; i < p_values.size(); i++)
		{
			(*this) << p_values[i];
		}
		return (*this);
	}

	jgl::IOStream& IOStream::operator << (const void* p_address)
	{
		std::ostringstream address;
		address << (void const*)this;

		std::string name = address.str();

		_addString(name);
		return (*this);
	}

	IOStream& endl(IOStream& os)
	{
		os.flush();
		os << '\n';
		return (os);
	}
}