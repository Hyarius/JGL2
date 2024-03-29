#pragma once

#include "jgl2_basic_types.h"
#include "jgl2_iostream.h"

namespace jgl
{
	struct Color
	{
		jgl::Float r;
		jgl::Float g;
		jgl::Float b;
		jgl::Float a;

		Color() :
			r(0),
			g(0),
			b(0),
			a(1)
		{

		}
		Color(jgl::Int p_value) : Color(p_value, p_value, p_value)
		{

		}

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

		Color operator + (const Color& p_color) const
		{
			Color result;

			result.r = std::clamp(r + p_color.r, 0.0f, 1.0f);
			result.g = std::clamp(g + p_color.g, 0.0f, 1.0f);
			result.b = std::clamp(b + p_color.b, 0.0f, 1.0f);

			return (result);
		}

		Color operator - (const Color& p_color) const
		{
			Color result;

			result.r = std::clamp(r - p_color.r, 0.0f, 1.0f);
			result.g = std::clamp(g - p_color.g, 0.0f, 1.0f);
			result.b = std::clamp(b - p_color.b, 0.0f, 1.0f);

			return (result);
		}

		friend jgl::IOStream& operator << (jgl::IOStream& p_os, const Color& p_values)
		{
			p_os << p_values.r << " / " << p_values.g << " / " << p_values.b << " / " << p_values.a;
			return (p_os);
		}
	};
}