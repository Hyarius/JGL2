#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define GET_VARIABLE_NAME(Variable) (#Variable)

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

namespace jgl
{
	std::vector<std::string> splitString(std::string p_text, jgl::Char p_delim);
	std::string glGetTypeString(GLenum type);
	void checkOpengl(std::string msg);
	std::string methodName(const std::string& prettyFunction);
	std::string className(const std::string& prettyFunction);
	jgl::Float degreeToRadian(const jgl::Float& angle);
	jgl::Float radianToDegree(const jgl::Float& radian);
	jgl::Long generateNumberFromSeed(const jgl::Long seed, const jgl::Long x, const jgl::Long y = 0, const jgl::Long z = 0);
	jgl::Long generateNumber(const jgl::Long min, const jgl::Long max);
	std::vector<std::string> stringSplit(const std::string& p_string, const std::string& p_delim);
	std::string getStr(std::fstream& p_file);
	std::vector<std::string> getStringSplit(std::fstream& p_file, const std::string& p_delim, const jgl::Int& p_expectedSize = -1);
	jgl::Int positive_modulo(jgl::Int i, jgl::Int n);
	std::string normalizeString(std::string p_str, jgl::Size_t p_finalLength, char p_char);
	std::vector<std::string> decomposeIntoParagraph(std::string p_str, jgl::Size_t p_lineLength, std::string p_delim);
	void editStringContent(std::string& p_str, jgl::Size_t p_position, jgl::Size_t p_length, char p_c);
	jgl::Size_t findCharFromEnd(std::string p_str, jgl::Size_t p_base, char p_c);
	std::vector<std::string> listFile(std::string p_path, std::string p_extension);

	template <typename T>
	std::string to_string(const T a_value, const jgl::Int n = 6)
	{
		std::ostringstream out;
		out.precision(n);
		out << std::fixed << a_value;
		return out.str();
	}
}

#define __METHOD__ jgl::methodName(__PRETTY_FUNCTION__)
#define __CLASS__ jgl::className(__PRETTY_FUNCTION__)

#define DEBUG_LINE() jgl::cout << __CLASS__ << " - " << __METHOD__ << "::" << __LINE__ << jgl::endl
#define DEBUG_LINE_TEXT(p_text) jgl::cout << __CLASS__ << " - " << __METHOD__ << "::" << __LINE__ << " - " << p_text << jgl::endl
