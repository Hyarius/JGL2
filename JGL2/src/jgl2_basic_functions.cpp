#include "jgl2_basic_functions.h"
#include "Structure/jgl2_iostream.h"
#include "jgl2_enums.h"

namespace jgl
{

	std::vector<std::string> splitString(std::string p_text, jgl::Char p_delim)
	{
		std::vector<std::string> result;

		std::string tmp;
		std::stringstream ss(p_text);

		std::istringstream input2(p_text);
		for (std::string word; std::getline(input2, word, p_delim);)
			result.push_back(word);

		return result;
	}

	std::string glGetTypeString(GLenum type)
	{
		switch (type)
		{
		case GL_NO_ERROR: return ("No error");
		case GL_INVALID_ENUM: return ("Invalid enum");
		case GL_INVALID_VALUE: return ("Invalid value");
		case GL_INVALID_OPERATION: return ("Invalid operation");
		case GL_INVALID_FRAMEBUFFER_OPERATION: return ("Invalid framebuffer operation");
		case GL_OUT_OF_MEMORY: return ("Out of memory");
		case GL_STACK_UNDERFLOW: return ("Stack underflow");
		case GL_STACK_OVERFLOW: return ("Stack overflow");

		case GL_ELEMENT_ARRAY_BUFFER: return ("GL_ELEMENT_ARRAY_BUFFER");
		case GL_ARRAY_BUFFER: return ("GL_ARRAY_BUFFER");
		case GL_BYTE: return ("GLbyte");
		case GL_UNSIGNED_BYTE: return ("GLubyte");
		case GL_UNSIGNED_SHORT: return ("GLushort");
		case GL_2_BYTES: return ("GL_2_BYTES");
		case GL_3_BYTES: return ("GL_3_BYTES");
		case GL_4_BYTES: return ("GL_4_BYTES");

		case 	GL_FLOAT: return ("float");
		case 	GL_FLOAT_VEC2: return ("vec2");
		case 	GL_FLOAT_VEC3: return ("vec3");
		case 	GL_FLOAT_VEC4: return ("vec4");
		case 	GL_DOUBLE: return ("double");
		case 	GL_DOUBLE_VEC2: return ("dvec2");
		case 	GL_DOUBLE_VEC3: return ("dvec3");
		case 	GL_DOUBLE_VEC4: return ("dvec4");
		case 	GL_INT: return ("int");
		case 	GL_INT_VEC2: return ("ivec2");
		case 	GL_INT_VEC3: return ("ivec3");
		case 	GL_INT_VEC4: return ("ivec4");
		case 	GL_UNSIGNED_INT: return ("unsigned int");
		case 	GL_UNSIGNED_INT_VEC2: return ("uvec2");
		case 	GL_UNSIGNED_INT_VEC3: return ("uvec3");
		case 	GL_UNSIGNED_INT_VEC4: return ("uvec4");
		case 	GL_BOOL: return ("bool");
		case 	GL_BOOL_VEC2: return ("bvec2");
		case 	GL_BOOL_VEC3: return ("bvec3");
		case 	GL_BOOL_VEC4: return ("bvec4");
		case 	GL_FLOAT_MAT2: return ("mat2");
		case 	GL_FLOAT_MAT3: return ("mat3");
		case 	GL_FLOAT_MAT4: return ("mat4");
		case 	GL_FLOAT_MAT2x3: return ("mat2x3");
		case 	GL_FLOAT_MAT2x4: return ("mat2x4");
		case 	GL_FLOAT_MAT3x2: return ("mat3x2");
		case 	GL_FLOAT_MAT3x4: return ("mat3x4");
		case 	GL_FLOAT_MAT4x2: return ("mat4x2");
		case 	GL_FLOAT_MAT4x3: return ("mat4x3");
		case 	GL_DOUBLE_MAT2: return ("dmat2");
		case 	GL_DOUBLE_MAT3: return ("dmat3");
		case 	GL_DOUBLE_MAT4: return ("dmat4");
		case 	GL_DOUBLE_MAT2x3: return ("dmat2x3");
		case 	GL_DOUBLE_MAT2x4: return ("dmat2x4");
		case 	GL_DOUBLE_MAT3x2: return ("dmat3x2");
		case 	GL_DOUBLE_MAT3x4: return ("dmat3x4");
		case 	GL_DOUBLE_MAT4x2: return ("dmat4x2");
		case 	GL_DOUBLE_MAT4x3: return ("dmat4x3");
		case 	GL_SAMPLER_1D: return ("sampler1D");
		case 	GL_SAMPLER_2D: return ("sampler2D");
		case 	GL_SAMPLER_3D: return ("sampler3D");
		case 	GL_SAMPLER_CUBE: return ("samplerCube");
		case 	GL_SAMPLER_1D_SHADOW: return ("sampler1DShadow");
		case 	GL_SAMPLER_2D_SHADOW: return ("sampler2DShadow");
		case 	GL_SAMPLER_1D_ARRAY: return ("sampler1DArray");
		case 	GL_SAMPLER_2D_ARRAY: return ("sampler2DArray");
		case 	GL_SAMPLER_1D_ARRAY_SHADOW: return ("sampler1DArrayShadow");
		case 	GL_SAMPLER_2D_ARRAY_SHADOW: return ("sampler2DArrayShadow");
		case 	GL_SAMPLER_2D_MULTISAMPLE: return ("sampler2DMS");
		case 	GL_SAMPLER_2D_MULTISAMPLE_ARRAY: return ("sampler2DMSArray");
		case 	GL_SAMPLER_CUBE_SHADOW: return ("samplerCubeShadow");
		case 	GL_SAMPLER_BUFFER: return ("samplerBuffer");
		case 	GL_SAMPLER_2D_RECT: return ("sampler2DRect");
		case 	GL_SAMPLER_2D_RECT_SHADOW: return ("sampler2DRectShadow");
		case 	GL_INT_SAMPLER_1D: return ("isampler1D");
		case 	GL_INT_SAMPLER_2D: return ("isampler2D");
		case 	GL_INT_SAMPLER_3D: return ("isampler3D");
		case 	GL_INT_SAMPLER_CUBE: return ("isamplerCube");
		case 	GL_INT_SAMPLER_1D_ARRAY: return ("isampler1DArray");
		case 	GL_INT_SAMPLER_2D_ARRAY: return ("isampler2DArray");
		case 	GL_INT_SAMPLER_2D_MULTISAMPLE: return ("isampler2DMS");
		case 	GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return ("isampler2DMSArray");
		case 	GL_INT_SAMPLER_BUFFER: return ("isamplerBuffer");
		case 	GL_INT_SAMPLER_2D_RECT: return ("isampler2DRect");
		case 	GL_UNSIGNED_INT_SAMPLER_1D: return ("usampler1D");
		case 	GL_UNSIGNED_INT_SAMPLER_2D: return ("usampler2D");
		case 	GL_UNSIGNED_INT_SAMPLER_3D: return ("usampler3D");
		case 	GL_UNSIGNED_INT_SAMPLER_CUBE: return ("usamplerCube");
		case 	GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: return ("usampler2DArray");
		case 	GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return ("usampler2DArray");
		case 	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: return ("usampler2DMS");
		case 	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return ("usampler2DMSArray");
		case 	GL_UNSIGNED_INT_SAMPLER_BUFFER: return ("usamplerBuffer");
		case 	GL_UNSIGNED_INT_SAMPLER_2D_RECT: return ("usampler2DRect");
		case 	GL_IMAGE_1D: return ("image1D");
		case 	GL_IMAGE_2D: return ("image2D");
		case 	GL_IMAGE_3D: return ("image3D");
		case 	GL_IMAGE_2D_RECT: return ("image2DRect");
		case 	GL_IMAGE_CUBE: return ("imageCube");
		case 	GL_IMAGE_BUFFER: return ("imageBuffer");
		case 	GL_IMAGE_1D_ARRAY: return ("image1DArray");
		case 	GL_IMAGE_2D_ARRAY: return ("image2DArray");
		case 	GL_IMAGE_2D_MULTISAMPLE: return ("image2DMS");
		case 	GL_IMAGE_2D_MULTISAMPLE_ARRAY: return ("image2DMSArray");
		case 	GL_INT_IMAGE_1D: return ("iimage1D");
		case 	GL_INT_IMAGE_2D: return ("iimage2D");
		case 	GL_INT_IMAGE_3D: return ("iimage3D");
		case 	GL_INT_IMAGE_2D_RECT: return ("iimage2DRect");
		case 	GL_INT_IMAGE_CUBE: return ("iimageCube");
		case 	GL_INT_IMAGE_BUFFER: return ("iimageBuffer");
		case 	GL_INT_IMAGE_1D_ARRAY: return ("iimage1DArray");
		case 	GL_INT_IMAGE_2D_ARRAY: return ("iimage2DArray");
		case 	GL_INT_IMAGE_2D_MULTISAMPLE: return ("iimage2DMS");
		case 	GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return ("iimage2DMSArray");
		case 	GL_UNSIGNED_INT_IMAGE_1D: return ("uimage1D");
		case 	GL_UNSIGNED_INT_IMAGE_2D: return ("uimage2D");
		case 	GL_UNSIGNED_INT_IMAGE_3D: return ("uimage3D");
		case 	GL_UNSIGNED_INT_IMAGE_2D_RECT: return ("uimage2DRect");
		case 	GL_UNSIGNED_INT_IMAGE_CUBE: return ("uimageCube");
		case 	GL_UNSIGNED_INT_IMAGE_BUFFER: return ("uimageBuffer");
		case 	GL_UNSIGNED_INT_IMAGE_1D_ARRAY: return ("uimage1DArray");
		case 	GL_UNSIGNED_INT_IMAGE_2D_ARRAY: return ("uimage2DArray");
		case 	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: return ("uimage2DMS");
		case 	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return ("uimage2DMSArray");
		case 	GL_UNSIGNED_INT_ATOMIC_COUNTER: return ("atomic_uint");


		default: return ("not a GLenum type");
		}
	}


	void checkOpengl(std::string msg)
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			std::string error = msg + " : type [" + glGetTypeString(err) + "]";
			jgl::cout << "Error opengl : " << error << jgl::endl;
			throw std::runtime_error(error.c_str());
		}
	}

	std::string to_string(const jgl::InputStatus& p_status)
	{
		switch (p_status)
		{
		case jgl::InputStatus::Down:
			return ("Down");
		case jgl::InputStatus::Up:
			return ("Up");
		case jgl::InputStatus::Released:
			return ("Released");
		case jgl::InputStatus::Pressed:
			return ("Pressed");
		}
		return ("Unknow");
	}

	std::string to_string(const VerticalAlignment& p_align)
	{
		switch (p_align)
		{
		case jgl::VerticalAlignment::Down:
			return ("Down");
		case jgl::VerticalAlignment::Centred:
			return ("Centred");
		case jgl::VerticalAlignment::Top:
			return ("Top");
		}
		return ("Unknow");
	}

	std::string to_string(const HorizontalAlignment& p_align)
	{
		switch (p_align)
		{
		case jgl::HorizontalAlignment::Left:
			return ("Left");
		case jgl::HorizontalAlignment::Centred:
			return ("Centred");
		case jgl::HorizontalAlignment::Right:
			return ("Right");
		}
		return ("Unknow");
	}
	
	std::string methodName(const std::string& prettyFunction)
	{
		size_t end = prettyFunction.rfind("(");
		size_t beginColon = prettyFunction.substr(0, end).rfind("::") + 2;
		size_t beginSpace = prettyFunction.substr(0, end).rfind(" ") + 1;
		size_t begin = std::max(beginColon, beginSpace);
		std::string result = prettyFunction.substr(begin, end - begin);
		return (result + "()");
	}

	std::string className(const std::string& prettyFunction)
	{
		size_t methodEnd = prettyFunction.rfind("(");
		size_t beginColon = prettyFunction.substr(0, methodEnd).rfind("::") + 2;
		size_t beginSpace = prettyFunction.substr(0, methodEnd).rfind(" ") + 1;
		size_t methodBegin = std::max(beginColon, beginSpace);
		jgl::Int closingBracket = 0;
		jgl::Size_t resultStart = 0;

		for (jgl::Int i = static_cast<jgl::Int>(methodBegin); i > 0; i--)
		{
			if (prettyFunction[i] == '>')
				closingBracket++;
			if (prettyFunction[i] == '<')
				closingBracket--;
			if ((prettyFunction[i] == ' ' || prettyFunction[i] == '\t') && closingBracket == 0)
				break;
			resultStart = i;
		}

		if (resultStart == methodBegin)
			return ("No class");

		return (prettyFunction.substr(resultStart, methodBegin - resultStart - 2));
	}

	jgl::Float degreeToRadian(const jgl::Float& angle)
	{
		return ((angle * static_cast<jgl::Float>(M_PI)) / 180.0f);
	}

	jgl::Float radianToDegree(const jgl::Float& radian)
	{
		return ((radian * 180.0f) / static_cast<jgl::Float>(M_PI));
	}

	jgl::Long generateNumberFromSeed(const jgl::Long seed, const jgl::Long x, const jgl::Long y, const jgl::Long z)
	{
		jgl::Size_t h = static_cast<jgl::UInt>(seed + static_cast<jgl::ULong>(x) * 374761393 + static_cast<jgl::ULong>(y) * 668265263); //all constants are prime
		h = (h ^ (h >> 13)) * 1274126177;
		return h ^ (h >> 16);
	}

	jgl::Long generateNumber(const jgl::Long min, const jgl::Long max)
	{
		if (min == max)
			return (min);

		jgl::Long value = 0;

		for (jgl::Size_t i = 0; i < 4; i++)
		{
			value = value << 8;
			jgl::UInt value_tmp = rand();
			value += value_tmp;
		}
		return((value % (max - min)) + min);
	}
}