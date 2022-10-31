#include "Structure/jgl2_string.h"

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <share.h>

namespace jgl
{
	String::String()
	{

	}

	String::String(const char* p_str)
	{
		for (jgl::Size_t i = 0; p_str[i] != '\0'; i++)
		{
			append(jgl::Glyph(p_str[i]));
		}
	}

	String::String(const std::string& p_str) : String(p_str.c_str())
	{
		
	}

	jgl::Size_t String::size() const
	{
		return (_size);
	}

	const jgl::Char* String::c_str() const
	{
		if (_str == nullptr || _edited == true)
		{
			if (_str == nullptr || _str_len <= _size)
			{
				if (_str != nullptr)
					delete[] _str;
				_str = new jgl::Char[_size + 1];
			}
			jgl::Size_t index = 0;
			for (jgl::Size_t i = 0; i < _content.size(); i++)
			{
				_str[index] = _content[i];
				index++;
				/*
				jgl::Glyph tmp_char = _content[i];
				for (jgl::Size_t j = 0; j < tmp_char.size(); j++)
				{
					_str[index] = tmp_char[j];
					index++;
				}
				*/
			}
			_str[_size] = '\0';
		}
		return (_str);
	}

	jgl::WChar* String::convertToWChar() const
	{
		jgl::WChar* result = new jgl::WChar[_content.size() + 1];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, result, _content.size() + 1, c_str(), _TRUNCATE);

		return (result);
	}

	jgl::Glyph& String::operator [] (jgl::Size_t p_index)
	{
		_edited = true;
		if (p_index >= _size)
			throw jgl::Exception(-1, "Index out of range");
		return (_content[p_index]);
	}

	const jgl::Glyph& String::operator [] (jgl::Size_t p_index) const
	{
		if (p_index >= _size)
			throw jgl::Exception(-1, "Index out of range");
		return (_content[p_index]);
	}

	void String::append(jgl::Glyph p_char, jgl::Size_t p_nb_char)
	{
		for (jgl::Size_t i = 0; i < p_nb_char; i++)
		{
			_content.push_back(p_char);
			_size += 1;// p_char.size();
		}
	}

	void String::append(jgl::String p_other)
	{
		for (jgl::Size_t i = 0; i < p_other.size(); i++)
		{
			append(p_other[i]);
		}
	}

	void String::push_back(jgl::Glyph p_char, jgl::Size_t p_nb_char)
	{
		append(p_char, p_nb_char);
	}

	void String::push_back(jgl::String p_other)
	{
		append(p_other);
	}

	String String::operator + (const String& p_other) const
	{
		String result;

		result.append(*this);
		result.append(p_other);

		return (result);
	}

	String& String::operator += (const Glyph& p_glyph)
	{
		this->append(p_glyph, 1);

		return (*this);
	}

	String& String::operator += (const String& p_other)
	{
		this->append(p_other);

		return (*this);
	}
}