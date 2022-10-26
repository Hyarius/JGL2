#pragma once

#include "jgl2_basic_types.h"

namespace jgl
{
	struct Color
	{
		jgl::Float r;
		jgl::Float g;
		jgl::Float b;
		jgl::Float a;

		Color(jgl::Int p_r, jgl::Int p_g, jgl::Int p_b, jgl::Int p_a = 255) :
			r(static_cast<jgl::Float>(p_r) / 255.0f),
			g(static_cast<jgl::Float>(p_g) / 255.0f),
			b(static_cast<jgl::Float>(p_b) / 255.0f),
			a(static_cast<jgl::Float>(p_a) / 255.0f)
		{

		}
		Color(jgl::Float p_r, jgl::Float p_g, jgl::Float p_b, jgl::Float p_a = 1.0f) :
			r(p_r),
			g(p_g),
			b(p_b),
			a(p_a)
		{

		}
	};
}