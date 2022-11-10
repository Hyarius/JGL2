#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "jgl2_iostream.h"

namespace jgl
{
	template <typename TType, jgl::Size_t C_NB_DIM>
	struct Vector
	{
		TType values[C_NB_DIM];

		Vector()
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				this->values[i] = {};
			}
		}

		TType& operator [] (jgl::Size_t p_index)
		{
			if (p_index >= C_NB_DIM)
				throw std::out_of_range(std::string(__FUNCTION__) + std::string("::") + std::to_string(__LINE__));
			return (this->values[p_index]);
		}

		const TType& operator [] (jgl::Size_t p_index) const
		{
			if (p_index >= C_NB_DIM)
				throw std::out_of_range(std::string(__FUNCTION__) + std::string("::") + std::to_string(__LINE__));
			return (this->values[p_index]);
		}

		template <typename TOtherType>
		Vector<TType, C_NB_DIM>& operator = (const Vector<TOtherType, C_NB_DIM>& p_other) const
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				values[i] = p_other.values[i];
			}
		}

		template <typename TOtherType>
		Vector<TType, C_NB_DIM> operator + (const Vector<TOtherType, C_NB_DIM>& p_other) const
		{
			Vector<TType, C_NB_DIM>  result;

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				result.values[i] = values[i] + p_other.values[i];
			}

			return (result);
		}

		template <typename TOtherType>
		Vector<TType, C_NB_DIM> operator - (const Vector<TOtherType, C_NB_DIM>& p_other) const
		{
			Vector<TType, C_NB_DIM>  result;

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				result.values[i] = values[i] - p_other.values[i];
			}

			return (result);
		}

		template <typename TOtherType>
		Vector<TType, C_NB_DIM> operator * (const Vector<TOtherType, C_NB_DIM>& p_other) const
		{
			Vector<TType, C_NB_DIM>  result;

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				result.values[i] = values[i] * p_other.values[i];
			}

			return (result);
		}

		template <typename TOtherType>
		Vector<TType, C_NB_DIM> operator / (const Vector<TOtherType, C_NB_DIM>& p_other) const
		{
			Vector<TType, C_NB_DIM>  result;

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				if (p_other.values[i] == 0)
					throw std::exception("Try to divide by 0");
				result.values[i] = values[i] / p_other.values[i];
			}

			return (result);
		}

		template <typename TOtherType>
		Vector<TType, C_NB_DIM>& operator += (const Vector<TOtherType, C_NB_DIM>& p_other)
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				values[i] = values[i] + p_other.values[i];
			}

			return (*this);
		}

		template <typename TOtherType>
		Vector<TType, C_NB_DIM>& operator -= (const Vector<TOtherType, C_NB_DIM>& p_other)
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				values[i] = values[i] - p_other.values[i];
			}

			return (*this);
		}

		template <typename TOtherType>
		Vector<TType, C_NB_DIM>& operator *= (const Vector<TOtherType, C_NB_DIM>& p_other)
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				values[i] = values[i] * p_other.values[i];
			}

			return (*this);
		}

		template <typename TOtherType>
		Vector<TType, C_NB_DIM>& operator /= (const Vector<TOtherType, C_NB_DIM>& p_other)
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				values[i] = values[i] / p_other.values[i];
			}

			return (*this);
		}

		template <typename TOtherType>
		jgl::Float distance(const Vector<TOtherType, C_NB_DIM>& p_values) const
		{
			jgl::Float result = 0;

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				TType tmp = values[i] - p_values.values[i];
				result += tmp * tmp;
			}

			return (std::sqrt(result));
		}

		jgl::Float length() const
		{
			jgl::Float result = 0;

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				result += values[i] * values[i];
			}

			return (std::sqrt(result));
		}

		Vector<jgl::Float, C_NB_DIM> normalize()
		{
			Vector<jgl::Float, C_NB_DIM> result;

			jgl::Float tmp_lenght = length();

			if (tmp_lenght == 0)
				throw std::exception("Try to divide by 0");

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				result.values[i] = values[i] / tmp_lenght;
			}

			return (result);
		}

		Vector<TType, C_NB_DIM> invert() const
		{
			Vector<TType, C_NB_DIM> result;

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				result.values[i] = values[i] * -1;
			}

			return (result);
		}

		template <typename TOtherType>
		bool operator == (const Vector<TOtherType, C_NB_DIM>& p_other) const
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				if (values[i] != p_other.values[i])
					return (false);
			}
			return (true);
		}

		template <typename TOtherType>
		bool operator != (const Vector<TOtherType, C_NB_DIM>& p_other) const
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				if (values[i] != p_other.values[i])
					return (true);
			}
			return (false);
		}

		template <typename TOtherType>
		bool operator < (const Vector<TOtherType, C_NB_DIM>& p_other) const
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				if (values[i] < p_other.values[i])
					return (true);
			}
			return (false);
		}

		template <typename TOtherType>
		bool operator > (const Vector<TOtherType, C_NB_DIM>& p_other) const
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				if (values[i] > p_other.values[i])
					return (true);
			}
			return (false);
		}

		template <typename TOtherType>
		bool operator <= (const Vector<TOtherType, C_NB_DIM>& p_other) const
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				if (values[i] > p_other.values[i])
					return (false);
			}
			return (true);
		}

		template <typename TOtherType>
		bool operator >= (const Vector<TOtherType, C_NB_DIM>& p_other) const
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				if (values[i] < p_other.values[i])
					return (false);
			}
			return (true);
		}

		std::string text() const
		{
			std::string result = "";

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				if (i != 0)
					result += " / ";
				std::string tmp = std::to_string(values[i]);
				result += tmp;
			}

			return (result);
		}

		template <typename TType, jgl::Size_t C_NB_DIM>
		static Vector<TType, C_NB_DIM> composeMin(const Vector<TType, C_NB_DIM>& p_valueA, const Vector<TType, C_NB_DIM>& p_valueB)
		{
			Vector<TType, C_NB_DIM> result;

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				result[i] = std::min(p_valueA[i], p_valueB[i]);
			}

			return (result);
		}

		template <typename TType, jgl::Size_t C_NB_DIM>
		static Vector<TType, C_NB_DIM> composeMax(const Vector<TType, C_NB_DIM>& p_valueA, const Vector<TType, C_NB_DIM>& p_valueB)
		{
			Vector<TType, C_NB_DIM> result;

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				result[i] = std::max(p_valueA[i], p_valueB[i]);
			}

			return (result);
		}

		friend jgl::IOStream& operator << (jgl::IOStream& p_os, const Vector<TType, C_NB_DIM>& p_values)
		{
			p_os << p_values.text();
			return (p_os);
		}
	};

	template <typename TType>
	struct BaseVector2 : public Vector<TType, 2>
	{
		TType& x() { return (this->values[0]); };
		TType& y() { return (this->values[1]); };
		const TType& x() const { return (this->values[0]); };
		const TType& y() const { return (this->values[1]); };

		BaseVector2() : Vector<TType, 2>()
		{

		}

		BaseVector2(TType p_x, TType p_y)
		{
			this->values[0] = p_x;
			this->values[1] = p_y;
		}

		template <typename TOtherType>
		BaseVector2(const Vector<TOtherType, 2>& p_other)
		{
			this->values[0] = p_other[0];
			this->values[1] = p_other[1];
		}

		BaseVector2<TType>& operator = (const BaseVector2<TType>& p_other)
		{
			for (jgl::Size_t i = 0; i < 2; i++)
			{
				this->values[i] = p_other.values[i];
			}

			return (*this);
		}
	};

	template <typename TType>
	struct BaseVector3 : public Vector<TType, 3>
	{
		TType& x() { return (this->values[0]); };
		TType& y() { return (this->values[1]); };
		TType& z() { return (this->values[2]); };

		BaseVector3() : Vector<TType, 3>()
		{

		}

		BaseVector3(TType p_x, TType p_y, TType p_z)
		{
			this->values[0] = p_x;
			this->values[1] = p_y;
			this->values[2] = p_z;
		}
		template <typename TOtherType>
		BaseVector3(const Vector<TOtherType, 2>& p_other, const TType& p_z)
		{
			this->values[0] = p_other[0];
			this->values[1] = p_other[1];
			this->values[2] = p_z;
		}

		template <typename TOtherType>
		BaseVector3(const Vector<TOtherType, 3>& p_other)
		{
			this->values[0] = p_other[0];
			this->values[1] = p_other[1];
			this->values[2] = p_other[2];
		}

		BaseVector3<TType>& operator = (const BaseVector2<TType>& p_other)
		{
			for (jgl::Size_t i = 0; i < 2; i++)
			{
				this->values[i] = p_other.values[i];
			}
			return (*this);
		}

		BaseVector3<TType>& operator = (const BaseVector3<TType>& p_other)
		{
			for (jgl::Size_t i = 0; i < 3; i++)
			{
				this->values[i] = p_other.values[i];
			}
			return (*this);
		}
	};

	template <typename TType>
	struct BaseVector4 : public Vector<TType, 4>
	{
		TType& x() { return (this->values[0]); };
		TType& y() { return (this->values[1]); };
		TType& z() { return (this->values[2]); };
		TType& w() { return (this->values[3]); };

		BaseVector4() : Vector<TType, 4>()
		{

		}

		BaseVector4(TType p_x, TType p_y, TType p_z, TType p_w)
		{
			this->values[0] = p_x;
			this->values[1] = p_y;
			this->values[2] = p_z;
			this->values[3] = p_w;
		}

		template <typename TOtherType>
		BaseVector4(const Vector<TOtherType, 2>& p_other, const TType& p_z, const TType& p_w)
		{
			this->values[0] = p_other[0];
			this->values[1] = p_other[1];
			this->values[2] = p_z;
			this->values[2] = p_w;
		}

		template <typename TOtherType>
		BaseVector4(const Vector<TOtherType, 3>& p_other, const TType& p_w)
		{
			this->values[0] = p_other[0];
			this->values[1] = p_other[1];
			this->values[2] = p_other[2];
			this->values[2] = p_w;
		}

		template <typename TOtherType>
		BaseVector4(const Vector<TOtherType, 4>& p_other)
		{
			this->values[0] = p_other[0];
			this->values[1] = p_other[1];
			this->values[2] = p_other[2];
			this->values[3] = p_other[3];
		}

		template <typename TOtherType>
		BaseVector4<TType>& operator = (const BaseVector2<TOtherType>& p_other)
		{
			for (jgl::Size_t i = 0; i < 2; i++)
			{
				this->values[i] = p_other.values[i];
			}
			return (*this);
		}
		template <typename TOtherType>
		BaseVector4<TType>& operator = (const BaseVector3<TOtherType>& p_other)
		{
			for (jgl::Size_t i = 0; i < 3; i++)
			{
				this->values[i] = p_other.values[i];
			}
			return (*this);
		}
		template <typename TOtherType>
		BaseVector4<TType>& operator = (const BaseVector4<TOtherType>& p_other)
		{
			for (jgl::Size_t i = 0; i < 4; i++)
			{
				this->values[i] = p_other.values[i];
			}
			return (*this);
		}
	};

	using Vector2 = BaseVector2<jgl::Float>;
	using Vector2Int = BaseVector2<jgl::Int>;
	using Vector2UInt = BaseVector2<jgl::UInt>;

	using Vector3 = BaseVector3<jgl::Float>;
	using Vector3Int = BaseVector3<jgl::Int>;
	using Vector3UInt = BaseVector3<jgl::UInt>;

	using Vector4 = BaseVector4<jgl::Float>;
	using Vector4Int = BaseVector4<jgl::Int>;
	using Vector4UInt = BaseVector4<jgl::UInt>;

	template <typename TType, jgl::Size_t C_NB_DIM>
	Vector<TType, C_NB_DIM> clamp(const Vector<TType, C_NB_DIM>& p_min, const Vector<TType, C_NB_DIM>& p_value, const Vector<TType, C_NB_DIM>& p_max)
	{
		Vector<TType, C_NB_DIM> result;

		for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
		{
			if (p_min[i] < p_max[i])
				result[i] = std::clamp(p_value[i], p_min[i], p_max[i]);
			else
				result[i] = std::clamp(p_value[i], p_max[i], p_min[i]);
		}

		return (result);
	}

	template <typename TType, jgl::Size_t C_NB_DIM>
	Vector<TType, C_NB_DIM> clamp(const Vector<TType, C_NB_DIM>& p_valueA, const Vector<TType, C_NB_DIM>& p_valueB)
	{
		return (clamp(p_valueA, p_valueA, p_valueB));
	}

}