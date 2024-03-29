#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

#include<locale>
#include <io.h>

namespace jgl
{
	class IOStream
	{
	public:
		static const jgl::Size_t C_BUFFER_SIZE = 1024;
	private:
		std::string _prefix;
		jgl::Bool _neededPrefix = true;
		jgl::Char _buffer[C_BUFFER_SIZE];
		jgl::Size_t _bufferLength = 0;
		static inline std::mutex _mutex;

		typedef IOStream& (*FormatFunctionPointer)(IOStream&);

		void _addChar(jgl::Char p_char);
		void _addString(std::string p_str);

	public:
		IOStream(const std::string& p_prefix);
		void clearConsole();
		void setPrefix(std::string p_prefix);
		void setEncoding(const char* locale, jgl::Int category = LC_CTYPE);
		virtual void flush();
		IOStream& operator << (const FormatFunctionPointer& p_funct);
		IOStream& operator << (const long p_value);
		IOStream& operator << (const jgl::Bool p_bool);
		IOStream& operator << (const jgl::Char p_char);
		IOStream& operator << (const jgl::UChar p_char);
		IOStream& operator << (const jgl::Char* p_str);
		IOStream& operator << (const jgl::UChar* p_str);
		IOStream& operator << (const jgl::Short p_value);
		IOStream& operator << (const jgl::UShort p_value);
		IOStream& operator << (const jgl::Int p_value);
		IOStream& operator << (const jgl::UInt p_value);
		IOStream& operator << (const jgl::Long p_value);
		IOStream& operator << (const jgl::ULong p_value);
		IOStream& operator << (const jgl::Float p_value);
		IOStream& operator << (const jgl::Double p_value);
		IOStream& operator << (const void* p_address);
		IOStream& operator << (const std::string p_string);
		template <const int TSize>
		IOStream& operator << (const std::bitset<TSize> p_bitset)
		{
			(*this) << p_bitset.to_string();
			return (*this);
		}

		template <typename TType>
		IOStream& operator << (const std::vector<TType> p_vector)
		{
			for (jgl::Size_t i = 0; i < p_vector.size(); i++)
			{
				if (i != 0)
					(*this) << " - ";
				(*this) << "[" << i << "]" << p_vector[i];
			}
			return (*this);
		}

	};

	extern thread_local jgl::IOStream cout;

	IOStream& endl(IOStream& os);
}

#ifndef _DEBUG // works in VS
#define DEBUG_TRACE(text) 
#else
#define DEBUG_TRACE(text) jgl::cout << text << jgl::endl
#endif
