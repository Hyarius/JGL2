#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

namespace jgl
{
	jgl::Float degreeToRadian(const jgl::Float& angle);
	jgl::Float radianToDegree(const jgl::Float& radian);

	template <typename TType>
	struct IVector2
	{
		TType x;
		TType y;

		IVector2() : x(0), y(0) { }

		IVector2(const TType& p_value) : x(p_value), y(p_value) {}

		IVector2(const TType& p_x, const TType& p_y) : x(p_x), y(p_y) {}

		template <typename TOtherType>
		operator IVector2<TOtherType>()
		{
			return (IVector2<TOtherType>(static_cast<TOtherType>(x), static_cast<TOtherType>(y)));
		}

		friend std::ostream& operator << (std::ostream& p_os, const IVector2& p_self)
		{
			p_os << p_self.x << " / " << p_self.y;
			return (p_os);
		}

		IVector2<TType> operator + (const IVector2& other) const
		{
			return (IVector2<TType>(x + other.x, y + other.y));
		}
		IVector2<TType> operator - (const IVector2& other) const
		{
			return (IVector2<TType>(x - other.x, y - other.y));
		}
		IVector2<TType> operator * (const IVector2& other) const
		{
			return (IVector2<TType>(x * other.x, y * other.y));
		}
		IVector2<TType> operator / (const IVector2& other) const
		{
			if (other.x == 0 || other.y == 0)
				throw std::runtime_error("Trying to divide by 0");
			return (IVector2<TType>(x / other.x, y / other.y));
		}
		void operator += (const IVector2& other)
		{
			x += other.x;
			y += other.y;
		}
		void operator -= (const IVector2& other)
		{
			x -= other.x;
			y -= other.y;
		}
		void operator *= (const IVector2& other)
		{
			x *= other.x;
			y *= other.y;
		}
		void operator /= (const IVector2& other)
		{
			if (other.x == 0 || other.y == 0)
				throw std::runtime_error("Trying to divide by 0");
			x /= other.x;
			y /= other.y;
		}

		jgl::Float length() const
		{
			return (sqrt(pow(x, 2.0f)
				+ pow(y, 2.0f)));
		}

		jgl::Float distance(const IVector2& other) const
		{
			return (std::sqrt(pow(other.x - x, 2) + pow(other.y - y, 2)));
		}

		IVector2<jgl::Float> normalize() const
		{
			jgl::Float tmp_length;

			tmp_length = length();

			if (tmp_length == 0)
				std::runtime_error("Trying to divide by 0");
			return (IVector2<jgl::Float>(static_cast<jgl::Float>(x) / tmp_length, static_cast<jgl::Float>(y) / tmp_length));
		}

		IVector2 cross(const IVector2& other = IVector2(0, 0)) const
		{
			IVector2 result;

			result = IVector2(-(other.y - y), other.x - x);

			return (result);
		}

		jgl::Float dot(const IVector2& other, const IVector2& center = IVector2(0, 0)) const
		{
			jgl::Float result;

			result = (x - center.x) * (other.x - center.x) + (y - center.y) * (other.y - center.y);

			return (result);
		}

		jgl::Float angle(const IVector2& other, const IVector2& center = IVector2(0, 0)) const
		{
			jgl::Float rdot = dot(other, center);

			rdot = (rdot < -1.0f ? -1.0f : (rdot > 1.0f ? 1.0f : rdot));

			jgl::Float angle = std::acos(rdot);

			return (jgl::radianToDegree(angle));
		}

		jgl::Float angle() const
		{
			jgl::Float degree = jgl::radianToDegree(std::atan(static_cast<jgl::Float>(y) / static_cast<jgl::Float>(x)));

			if (x >= 0 && y >= 0)
				return (degree);
			else if (x >= 0 && y < 0)
				return (360 + degree);
			else if (x < 0 && y >= 0)
				return (180 + degree);
			else
				return (180 + degree);
		}

		IVector2 invert() const
		{
			return (IVector2(x * -1, y * -1));
		}

		IVector2 rotate(IVector2 center, jgl::Float angle) const
		{
			float theta = jgl::degreeToRadian(angle);

			float pcos = cos(theta);
			float psin = sin(theta);

			jgl::IVector2 result;
			result.x = (x - center.x) * pcos - (y - center.y) * psin;
			result.y = (x - center.x) * psin + (y - center.y) * pcos;

			return (result + center);
		}

		bool operator == (const IVector2& delta) const
		{
			return ((x == delta.x && y == delta.y) ? true : false);
		}
		bool operator != (const IVector2& delta) const
		{
			return ((x == delta.x && y == delta.y) ? false : true);
		}
		bool operator < (const IVector2& other) const
		{
			if (y < other.y)
				return (true);
			if (y == other.y && x < other.x)
				return (true);
			return (false);
		}
		bool operator > (const IVector2& other) const
		{
			if (y > other.y)
				return (true);
			if (y == other.y && x > other.x)
				return (true);
			return (false);
		}
		bool operator <= (const IVector2& other) const
		{
			if (this->operator == (other) == true || this->operator < (other) == true)
				return (true);
			return (false);
		}
		bool operator >= (const IVector2& other) const
		{
			if (this->operator == (other) == true || this->operator > (other) == true)
				return (true);
			return (false);
		}

		static IVector2 max(const IVector2& p_lhd, const IVector2& p_rhd)
		{
			IVector2 result;

			result.x = std::max(p_lhd.x, p_rhd.x);
			result.y = std::max(p_lhd.y, p_rhd.y);

			return (result);
		}

		static IVector2 min(const IVector2& p_lhd, const IVector2& p_rhd)
		{
			IVector2 result;

			result.x = std::min(p_lhd.x, p_rhd.x);
			result.y = std::min(p_lhd.y, p_rhd.y);

			return (result);
		}

		static IVector2 clamp(const IVector2& p_min, const IVector2& p_value, const IVector2& p_max)
		{
			IVector2 result;

			result.x = std::max(p_min.x, std::min(p_max.x, p_value.x));
			result.y = std::max(p_min.y, std::min(p_max.y, p_value.y));

			return (result);
		}

		IVector2 floor()
		{
			IVector2 result;

			result.x = std::floorf(x);
			result.y = std::floorf(y);

			return (result);
		}
		IVector2 ceiling()
		{
			IVector2 result;

			result.x = std::ceilf(x);
			result.y = std::ceilf(y);

			return (result);
		}
		IVector2 round()
		{
			IVector2 result;

			result.x = std::roundf(x);
			result.y = std::roundf(y);

			return (result);
		}
	};

	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector2<TType> operator+(TValueType p_value, const IVector2<TType>& p_point)
	{
		return (IVector2<TType>(p_value, p_value) + p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector2<TType> operator-(TValueType p_value, const IVector2<TType>& p_point)
	{
		return (IVector2<TType>(p_value, p_value) - p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector2<TType> operator*(TValueType p_value, const IVector2<TType>& p_point)
	{
		return (IVector2<TType>(p_value, p_value) * p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector2<TType> operator/(TValueType p_value, const IVector2<TType>& p_point)
	{
		return (IVector2<TType>(p_value, p_value) / p_point);
	};

	using Vector2 = IVector2<jgl::Float>;
	using Vector2Int = IVector2<jgl::Int>;
	using Vector2UInt = IVector2<jgl::UInt>;
};