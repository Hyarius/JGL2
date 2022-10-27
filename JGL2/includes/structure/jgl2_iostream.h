#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

#include "jgl2_string.h"

#include<locale>
#include <io.h>

namespace jgl
{
	class IOStream
	{
	public:
		static const jgl::Size_t C_BUFFER_SIZE = 1024;
	private:
		jgl::String _prefix;
		jgl::Bool _needed_prefix = true;
		jgl::Char _buffer[C_BUFFER_SIZE];
		jgl::Size_t _buffer_len = 0;
		static inline std::mutex _mutex;

		typedef IOStream& (*FormatFunctionPointer)(IOStream&);

		void _addChar(jgl::Char p_char);
		void _addString(std::string p_str);

	public:
		IOStream(const jgl::String& p_prefix);
		void setEncoding(const char* locale, jgl::Int category = LC_CTYPE);
		virtual void flush();
		IOStream& operator << (const FormatFunctionPointer& p_funct);
		IOStream& operator << (const jgl::Char p_char);
		IOStream& operator << (const jgl::UChar p_char);
		IOStream& operator << (const jgl::Char* p_str);
		IOStream& operator << (const jgl::Short p_value);
		IOStream& operator << (const jgl::UShort p_value);
		IOStream& operator << (const jgl::Int p_value);
		IOStream& operator << (const jgl::UInt p_value);
		IOStream& operator << (const jgl::Long p_value);
		IOStream& operator << (const jgl::ULong p_value);
		IOStream& operator << (const jgl::Float p_value);
		IOStream& operator << (const jgl::Double p_value);
		//jgl::IOStream& operator << (const Glyph& p_glyph);
		jgl::IOStream& operator << (const String& p_values);
		jgl::IOStream& operator << (const void* p_address);
	};

	extern thread_local jgl::IOStream cout;

	IOStream& endl(IOStream& os);
}