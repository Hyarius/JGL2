#include "structure/jgl2_iostream.h"

namespace jgl
{
	thread_local jgl::IOStream cout = jgl::IOStream("Main");

	void IOStream::_addChar(jgl::Char p_char)
	{
		if (_bufferLength >= C_BUFFER_SIZE)
		{
			flush();
		}

		if (p_char == '\n')
		{
			flush();
		}
		else
		{
			_buffer[_bufferLength] = p_char;
			_bufferLength++;
		}
	}
	void IOStream::_addString(std::string p_str)
	{
		for (jgl::Size_t i = 0; i < p_str.size(); i++)
		{
			_addChar(p_str[i]);
		}
	}

	IOStream::IOStream(const std::string& p_prefix) : _prefix(p_prefix)
	{

	}

	void IOStream::setEncoding(const char* locale, jgl::Int category)
	{
		setlocale(category, locale);
	}

	void IOStream::flush()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		if (_neededPrefix == true)
		{
			int first = _write(1, "[", 1);
			int second = _write(1, _prefix.c_str(), static_cast<jgl::UInt>(_prefix.size()));
			int thrid = _write(1, "] - ", 4);
			_neededPrefix = false;
		}
		int forth = _write(1, _buffer, _bufferLength);
		int fifth = _write(1, "\n", 1);
		_neededPrefix = true;
		_bufferLength = 0;
	}

	IOStream& IOStream::operator << (const FormatFunctionPointer& p_funct)
	{
		return (*p_funct)(*this);
	}

	IOStream& IOStream::operator << (const long p_value)
	{
		_addString(std::to_string(p_value));
		return (*this);
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

	IOStream& IOStream::operator << (const jgl::WChar* p_str)
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

	jgl::IOStream& IOStream::operator << (const void* p_address)
	{
		std::ostringstream address;
		address << (void const*)this;

		std::string name = address.str();

		_addString(name);
		return (*this);
	}

	IOStream& IOStream::operator << (const std::string p_string)
	{
		for (jgl::Size_t i = 0; i < p_string.size(); i++)
		{
			(*this) << p_string[i];
		}
		return (*this);
	}

	IOStream& endl(IOStream& os)
	{
		os.flush();
		return (os);
	}
}