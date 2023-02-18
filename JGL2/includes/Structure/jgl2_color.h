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

		Color(jgl::Float p_value) : Color(p_value, p_value, p_value)
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

		friend std::ostream& operator << (std::ostream& p_os, const Color& p_self)
		{
			p_os << p_self.r << " / " << p_self.g << " / " << p_self.b << " / " << p_self.a;
			return (p_os);
		}

		Color operator + (const Color& p_other) const
		{
			Color result = *this;

			result += p_other;

			return (result);
		}

		Color operator - (const Color& p_other) const
		{
			Color result = *this;

			result -= p_other;

			return (result);
		}

		Color operator * (const Color& p_other) const
		{
			Color result = *this;

			result *= p_other;

			return (result);
		}

		void operator += (const Color& p_other)
		{
			r = std::clamp(r + p_other.r, 0.0f, 1.0f);
			g = std::clamp(g + p_other.g, 0.0f, 1.0f);
			b = std::clamp(b + p_other.b, 0.0f, 1.0f);
		}

		void operator -= (const Color& p_other)
		{
			r = std::clamp(r - p_other.r, 0.0f, 1.0f);
			g = std::clamp(g - p_other.g, 0.0f, 1.0f);
			b = std::clamp(b - p_other.b, 0.0f, 1.0f);
		}

		void operator *= (const Color& p_other)
		{
			jgl::Float	base_alpha;
			jgl::Float	inv_alpha;

			base_alpha = p_other.a;
			inv_alpha = 1.0f - base_alpha;

			r = p_other.r * base_alpha + inv_alpha * r;
			g = p_other.g * base_alpha + inv_alpha * g;
			b = p_other.b * base_alpha + inv_alpha * b;
			a = p_other.a * base_alpha + inv_alpha * a;
		}

		static jgl::Color transparent;
		static jgl::Color white;
		static jgl::Color black;
		static jgl::Color red;
		static jgl::Color green;
		static jgl::Color blue;
	};
}