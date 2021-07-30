#include "jgl2.h"

jgl::Exception e_exception;

namespace jgl
{
	jgl::Int jgl::String::npos = -1;
	Glyph jgl::String::nullchar = '\0';
	jgl::Error_level Exception::_minimal_error_level = jgl::Error_level::Error;


	jgl::String glGetTypeString(GLenum type)
	{
		switch (type)
		{
		case GL_ELEMENT_ARRAY_BUFFER: return ("GL_ELEMENT_ARRAY_BUFFER");
		case GL_ARRAY_BUFFER: return ("GL_ARRAY_BUFFER");
		case GL_BYTE: return "GLbyte";
		case GL_UNSIGNED_BYTE: return "GLubyte";
		case GL_SHORT: return "GLshort";
		case GL_UNSIGNED_SHORT: return "GLushort";
		case GL_INT: return "GLint";
		case GL_UNSIGNED_INT: return "GLuint";
		case GL_FLOAT: return "GLjgl::Float";
		case GL_2_BYTES: return "GL_2_BYTES";
		case GL_3_BYTES: return "GL_3_BYTES";
		case GL_4_BYTES: return "GL_4_BYTES";
		case GL_DOUBLE: return "GLdouble";
		default: return "not a GLenum type";
		}
	}

	jgl::Int start()
	{
		bool is_started = true;

		if (is_started == false)
			THROW_EXCEPTION(jgl::Error_level::Critical, 0, "Error while starting JGL data");

		SetConsoleOutputCP(1252);
		SetConsoleCP(1252);

		THROW_INFORMATION("Encoding set to allow UTF16");

		return (0);
	}

	jgl::Int stop()
	{
		bool quit_completed = true;

		if (quit_completed == false)
			THROW_EXCEPTION(jgl::Error_level::Critical, -2, "Error while quiting JGL data");

		THROW_INFORMATION("Closing of jgl complete");
		return (0);
	}

	void throw_exception(jgl::Error_level level, jgl::Int id, jgl::String msg, const char* file, const char* function, jgl::Int line)
	{
		e_exception.set_level(level);
		e_exception.set_file(jgl::String(__FILE__));
		e_exception.set_function(jgl::String(__FUNCTION__));
		e_exception.set_line(__LINE__);
		e_exception.define(id, msg);
		std::cout << e_exception.partial() << std::endl;
		if (static_cast<jgl::Int>(jgl::Exception::minimal_error_level()) <= static_cast<jgl::Int>(level))
		{
			throw (e_exception);
		}
	}

	const Exception& get_error()
	{
		return (e_exception);
	}

	jgl::Float				degree_to_radian(const jgl::Float angle)
	{
		return ((angle * static_cast<jgl::Float>(M_PI)) / 180.0f);
	}

	jgl::Float				radian_to_degree(const jgl::Float radian)
	{
		return ((radian * 180.0f) / static_cast<jgl::Float>(M_PI));
	}

	jgl::Int floor(jgl::Float value)
	{
		return (static_cast<jgl::Int>(std::floor(value)));
	}

	jgl::Int round(jgl::Float value)
	{
		return (static_cast<jgl::Int>(std::round(value)));
	}

	jgl::Int ceil(jgl::Float value)
	{
		return (static_cast<jgl::Int>(std::ceil(value)));
	}

	jgl::String normalize_string(const jgl::String& str, const jgl::Glyph c, const jgl::Size_t size)
	{
		jgl::String result;

		result = str;

		while (result.size() < size)
			result.insert(c, 0);

		return (result);
	}

	jgl::String normalize_int(const jgl::Int num, const jgl::Glyph c, const jgl::Size_t size)
	{
		jgl::String result;

		result = itoa(num);
		while (result.size() < size)
			result.insert(c, 0);

		return (result);
	}

	jgl::String normalize_uint(const jgl::Uint num, const jgl::Glyph c, const jgl::Size_t size)
	{
		jgl::String result;

		result = itoa(num);
		while (result.size() < size)
			result.insert(c, 0);

		return (result);
	}

	jgl::String normalize_float(const jgl::Float num, const jgl::Int after_point, const jgl::Glyph c, const jgl::Size_t size)
	{
		jgl::String result;

		result = ftoa(num, after_point);
		while (result.size() < size)
			result.insert(c, 0);

		return (result);
	}

	void reverse(jgl::String& base)
	{
		jgl::Size_t i = 0, j = base.size() - 1;
		jgl::Glyph temp;
		while (i < j)
		{
			temp = base[i];
			base[i] = base[j];
			base[j] = temp;
			i++; j--;
		}
	}

	jgl::String itoa(const jgl::Int x, const jgl::Int d)
	{
		std::string result = std::to_string(x);
		size_t i = result.size();

		if (d > 0)
		{
			while (i < (uint32_t)d)
			{
				result.insert(0, " ");
				i++;
			}
		}

		return (jgl::String(result));
	}

	// Converts a jgl::Floating pojgl::Int number tostring.
	jgl::String ftoa(const jgl::Float n, const jgl::Int afterpoint, const jgl::Int length)
	{
		std::string text;
		std::stringstream out;
		jgl::Int tmp = afterpoint;

		if (n > 0 || n < -1)
			tmp += 2;
		out << std::setprecision(tmp) << n;
		text = out.str();
		while (length != -1 && text.length() < static_cast<jgl::Size_t>(length))
			text.insert(text.begin(), ' ');
		out.seekp(0);
		out << text;
		return (jgl::String(out.str()));
	}
	// Converts a floating pojgl::Int number tostring.
	jgl::String ftoa(const jgl::Double n, const jgl::Int afterpoint, const jgl::Int length)
	{
		return (ftoa((jgl::Float)n, afterpoint, length));
	}


	jgl::Float stof(jgl::String text)
	{
		const char* tmp = text.c_str();
		return (static_cast<jgl::Float>(std::atof(tmp)));
	}
	jgl::Int stoi(jgl::String text)
	{
		const char* tmp = text.c_str();
		return (std::atoi(tmp));
	}

	bool is_middle(const jgl::Int min, const jgl::Int value, const jgl::Int max)
	{
		if (value < min)
			return (false);
		if (value >= max)
			return (false);
		return (true);
	}

	bool is_middle(const jgl::Float min, const jgl::Float value, const jgl::Float max)
	{
		if (value < min)
			return (false);
		if (value >= max)
			return (false);
		return (true);
	}

	bool is_middle(const jgl::Vector2 min, const jgl::Vector2 value, const jgl::Vector2 max)
	{
		if (is_middle(min.x, value.x, max.x) == true && is_middle(min.y, value.y, max.y) == true)
			return (true);
		return (false);
	}

	bool is_middle(const jgl::Vector3 min, const jgl::Vector3 value, const jgl::Vector3 max)
	{
		if (is_middle(min.x, value.x, max.x) == true && is_middle(min.y, value.y, max.y) == true && is_middle(min.z, value.z, max.z) == true)
			return (true);
		return (false);
	}

	void remove_char(jgl::String& src, jgl::String& to_remove)
	{
		jgl::String result;
		for (jgl::Size_t i = 0; i < src.size(); i++)
		{
			jgl::Glyph target = '\0';
			for (jgl::Size_t j = 0; j < to_remove.size(); j++)
				if (to_remove[j] == src[i])
					target = to_remove[j];

			if (target == '\0')
				result.push_back(src[i]);
		}
		src = result;
	}

	jgl::Int generate_nbr_from_2D(const jgl::Long seed, const jgl::Int x, const jgl::Int y)
	{
		jgl::Size_t h = static_cast<jgl::Uint>(seed + static_cast<jgl::Ulong>(x) * 374761393 + static_cast<jgl::Ulong>(y) * 668265263); //all constants are prime
		h = (h ^ (h >> 13)) * 1274126177;
		return h ^ (h >> 16);
	}

	jgl::Int generate_nbr(const jgl::Int min, const jgl::Int max)
	{
		return((rand() % (max - min)) + min);
	}
}