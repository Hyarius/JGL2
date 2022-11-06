#pragma once

#include "jgl2_includes.h"

namespace jgl
{
	enum class InputStatus
	{
		Down = 0,
        Released = 1,
		Up = 2,
		Pressed = 3,
	};

	std::string to_string(const InputStatus& p_status);
}