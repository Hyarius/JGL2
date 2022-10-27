#include "Structure/jgl2_glyph.h"


#include <bitset>
#include <iostream>

namespace jgl
{
	/*
	jgl::Int Glyph::_countCharLen(const jgl::Char p_toParse)
	{
		return (1);
	}

	void Glyph::_decodeUnicodeChar(const jgl::Char* p_str)
	{
		jgl::Int limit = _countCharLen(p_str[0]);

		if (limit == -1)
		{
			throw jgl::Exception(-1, "Unexpected encoding char in wide char parsing operation");
		}

		_size = limit;
		for (jgl::Int i = 0; i < limit; i++)
			_content[i] = p_str[i];
		_content[limit] = '\0';
	}

	Glyph::Glyph()
	{
		clean();
	}

	Glyph::Glyph(jgl::WChar p_c) : Glyph()
	{
		_decodeUnicodeChar(reinterpret_cast<jgl::Char*>(&p_c));
	}

	jgl::Size_t Glyph::size() const
	{
		return (_size);
	}

	const char& Glyph::operator [] (jgl::Size_t p_index) const
	{
		if (p_index >= _size)
			throw jgl::Exception(-1, "Index out of range");
		return (_content[p_index]);
	}

	void Glyph::clean()
	{
		for (jgl::Size_t i = 0; i < 5; i++)
		{
			_content[i] = '\0';
		}
	}
	*/
}