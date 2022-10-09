#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "jgl2_glyph.h"

namespace jgl
{
	class String
	{
	private:
		jgl::Size_t _size = 0;
		std::vector<jgl::Glyph> _content;

		mutable jgl::Bool _edited = false;
		mutable jgl::Char* _str = nullptr;
		mutable jgl::Size_t _str_len = 0;

	public:
		String();
		String(const char* p_str);

		jgl::Size_t size() const;
		const char* c_str() const;

		jgl::Glyph& operator [] (jgl::Size_t p_index);
		const jgl::Glyph& operator [] (jgl::Size_t p_index) const;

		void append(jgl::Glyph p_char, jgl::Size_t p_nb_char = 1);
		void append(jgl::String p_other);
		void push_back(jgl::Glyph p_char, jgl::Size_t p_nb_char = 1);
		void push_back(jgl::String p_other);
		String operator + (const String& p_other) const;
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