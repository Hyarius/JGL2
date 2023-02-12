#pragma once

#include "jgl2_basic_types.h"
#include "jgl2_iostream.h"

namespace jgl
{
	struct Color
	{
		float r;
		float g;
		float b;
		float a;

		Color() :
			r(0),
			g(0),
			b(0),
			a(1)
		{

		}

		Color(float p_value) : Color(p_value, p_value, p_value)
		{

		}

		Color(int p_value) : Color(p_value, p_value, p_value)
		{

		}

		Color(int p_r, int p_g, int p_b, int p_a = 255) :
			r(static_cast<float>(p_r) / 255.0f),
			g(static_cast<float>(p_g) / 255.0f),
			b(static_cast<float>(p_b) / 255.0f),
			a(static_cast<float>(p_a) / 255.0f)
		{

		}

		Color(float p_r, float p_g, float p_b, float p_a = 1.0f) :
			r(p_r),
			g(p_g),
			b(p_b),
			a(p_a)
		{

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
			float	base_alpha;
			float	inv_alpha;

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