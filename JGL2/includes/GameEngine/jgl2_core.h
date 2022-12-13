#pragma once

#include "jgl2.h"

namespace jgl
{
	struct Core
	{
		std::string name;
		jgl::Bool isActive;
		jgl::Vector3 position;
		jgl::Vector3 size;
		jgl::Vector3 forward;

		Core(std::string p_name = "Unnamed") :
			name(p_name),
			isActive(false),
			position(0, 0, 0),
			size(1, 1, 1),
			forward(0, -1, 0)
		{

		}
	};
}