#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

namespace jgl
{
	std::vector<std::string> splitString(std::string p_text, jgl::Char p_delim);
	std::string glGetTypeString(GLenum type);
	void checkOpengl(std::string msg);
}