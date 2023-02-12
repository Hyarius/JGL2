#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "structure/jgl2_vector2.h"
#include "structure/jgl2_vector3.h"

namespace jgl
{
	template <typename TType>
	struct IVector4
	{
		TType x;
		TType y;
		TType z;
		TType w;

		IVector4() : x(0), y(0), z(0), w(0) { }

		IVector4(const TType& p_value) : x(p_value), y(p_value), z(p_value), w(p_value) {}

		IVector4(const IVector2<TType>& p_other, const TType& p_value) : x(p_other.x), y(p_other.y), z(p_value), w(p_value) {}

		IVector4(const IVector3<TType>& p_other, const TType& p_value) : x(p_other.x), y(p_other.y), z(p_other.z), w(p_value) {}

		IVector4(const TType& p_x, const TType& p_y, const TType& p_z, const TType& p_w) : x(p_x), y(p_y), z(p_z), w(p_w) {}

		template <typename TOtherType>
		operator IVector4<TOtherType>()
		{
			return (IVector4<TOtherType>(static_cast<TOtherType>(x), static_cast<TOtherType>(y), static_cast<TOtherType>(z), static_cast<TOtherType>(w)));
		}

		friend std::ostream& operator << (std::ostream& p_os, const IVector4& p_self)
		{
			p_os << p_self.x << " / " << p_self.y << " / " << p_self.z << " / " << p_self.w;
			return (p_os);
		}

		IVector4<TType> operator + (const IVector4& other) const
		{
			return (IVector4<TType>(x + other.x, y + other.y, z + other.z, w + other.w));
		}
		IVector4<TType> operator - (const IVector4& other) const
		{
			return (IVector4<TType>(x - other.x, y - other.y, z - other.z, w - other.w));
		}
		IVector4<TType> operator * (const IVector4& other) const
		{
			return (IVector4<TType>(x * other.x, y * other.y, z * other.z, w * other.w));
		}
		IVector4<TType> operator / (const IVector4& other) const
		{
			if (other.x == 0 || other.y == 0 || other.z == 0 || other.w == 0)
				throw std::runtime_error("Trying to divide by 0");
			return (IVector4<TType>(x / other.x, y / other.y, z / other.z, w / other.w));
		}
		void operator += (const IVector4& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
		}
		void operator -= (const IVector4& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
		}
		void operator *= (const IVector4& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
		}
		void operator /= (const IVector4& other)
		{
			if (other.x == 0 || other.y == 0 || other.z == 0 || other.w == 0)
				throw std::runtime_error("Trying to divide by 0");
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
		}

		bool operator == (const IVector4& delta) const
		{
			return ((x == delta.x && y == delta.y && z == delta.z && w == delta.w) ? true : false);
		}
		bool operator != (const IVector4& delta) const
		{
			return ((x == delta.x && y == delta.y && z == delta.z && w == delta.w) ? false : true);
		}
		bool operator < (const IVector4& other) const
		{
			if (w < other.w)
				return (true);
			if (w == other.w && z < other.z)
				return (true);
			if (w == other.w && z == other.z && y < other.y)
				return (true);
			if (w == other.w && z == other.z && y == other.y && x < other.x)
				return (true);
			return (false);
		}
		bool operator > (const IVector4& other) const
		{
			if (w > other.w)
				return (true);
			if (w == other.w && z > other.z)
				return (true);
			if (w == other.w && z == other.z && y > other.y)
				return (true);
			if (w == other.w && z == other.z && y == other.y && x > other.x)
				return (true);
			return (false);
		}
		bool operator <= (const IVector4& other) const
		{
			if (this->operator == (other) == true || this->operator < (other) == true)
				return (true);
			return (false);
		}
		bool operator >= (const IVector4& other) const
		{
			if (this->operator == (other) == true || this->operator > (other) == true)
				return (true);
			return (false);
		}

		static IVector4 max(const IVector4& p_lhd, const IVector4& p_rhd)
		{
			IVector4 result;

			result.x = std::max(p_lhd.x, p_rhd.x);
			result.y = std::max(p_lhd.y, p_rhd.y);
			result.z = std::max(p_lhd.z, p_rhd.z);
			result.w = std::max(p_lhd.w, p_rhd.w);

			return (result);
		}

		static IVector4 min(const IVector4& p_lhd, const IVector4& p_rhd)
		{
			IVector4 result;

			result.x = std::min(p_lhd.x, p_rhd.x);
			result.y = std::min(p_lhd.y, p_rhd.y);
			result.z = std::min(p_lhd.z, p_rhd.z);
			result.w = std::min(p_lhd.w, p_rhd.w);

			return (result);
		}

		static IVector4 clamp(const IVector4& p_min, const IVector4& p_value, const IVector4& p_max)
		{
			IVector4 result;

			result.x = std::max(p_min.x, std::min(p_max.x, p_value.x));
			result.y = std::max(p_min.y, std::min(p_max.y, p_value.y));
			result.z = std::max(p_min.z, std::min(p_max.z, p_value.z));
			result.w = std::max(p_min.w, std::min(p_max.w, p_value.w));

			return (result);
		}
	};

	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector4<TType> operator+(TValueType p_value, const IVector4<TType>& p_point)
	{
		return (IVector4<TType>(p_value) + p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector4<TType> operator-(TValueType p_value, const IVector4<TType>& p_point)
	{
		return (IVector4<TType>(p_value) - p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector4<TType> operator*(TValueType p_value, const IVector4<TType>& p_point)
	{
		return (IVector4<TType>(p_value) * p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector4<TType> operator/(TValueType p_value, const IVector4<TType>& p_point)
	{
		return (IVector4<TType>(p_value) / p_point);
	};

	using Vector4 = IVector4<jgl::Float>;
	using Vector4Int = IVector4<jgl::Int>;
	using Vector4UInt = IVector4<jgl::UInt>;
};