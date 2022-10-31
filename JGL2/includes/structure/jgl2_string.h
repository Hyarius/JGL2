#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "jgl2_glyph.h"

namespace jgl
{
	class String
	{
	private:
		Size_t _size = 0;
		std::vector<Glyph> _content;

		mutable Bool _edited = false;
		mutable Char* _str = nullptr;
		mutable Size_t _str_len = 0;
		mutable WChar* _wstr = nullptr;
		mutable Size_t _wstr_len = 0;

	public:
		String();
		String(const char* p_str);
		String(const std::string& p_str);

		Size_t size() const;
		const Char* c_str() const;
		WChar* convertToWChar() const;

		Glyph& operator [] (Size_t p_index);
		const Glyph& operator [] (Size_t p_index) const;

		void append(Glyph p_char, Size_t p_nb_char = 1);
		void append(String p_other);
		void push_back(Glyph p_char, Size_t p_nb_char = 1);
		void push_back(String p_other);
		String operator + (const String& p_other) const;
		String& operator += (const Glyph& p_glyph);
		String& operator += (const String& p_other);

		friend String operator + (const char* p_first, const String& p_second)
		{
			String result;

			for (Size_t i = 0; p_first[i] != '\0'; i++)
				result.append(p_first[i]);

			for (Size_t i = 0; i < p_second.size(); i++)
				result.append(p_second[i]);

			return (result);
		}
	};
}