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
}

#define __METHOD__ jgl::methodName(__PRETTY_FUNCTION__)
#define __CLASS__ jgl::className(__PRETTY_FUNCTION__)

#define DEBUG_LINE() jgl::cout << __CLASS__ << " - " << __METHOD__ << " : " << __FUNCTION__ << "::" << __LINE__ << jgl::endl
#define DEBUG_LINE_TEXT(p_text) jgl::cout << __CLASS__ << " - " << __METHOD__ << " : " << __FUNCTION__ << "::" << __LINE__ << " - " << p_text << jgl::endl
