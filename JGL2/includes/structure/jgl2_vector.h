#pragma once

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
				values[i] = {};
			}
		}
		TType& operator [] (jgl::Size_t p_index)
		{
			if (p_index >= C_NB_DIM)
				throw jgl::Exception("Index out of range");
			return (values[p_index]);
		}

		const TType& operator [] (jgl::Size_t p_index) const
		{
			if (p_index >= C_NB_DIM)
				throw jgl::Exception("Index out of range");
			return (values[p_index]);
		}

		Vector<TType, C_NB_DIM>& operator = (const Vector<TType, C_NB_DIM>& p_other) const
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				values[i] = p_other.values[i];
			}
		}

		Vector<TType, C_NB_DIM> operator + (const Vector<TType, C_NB_DIM>& p_other) const
		{
			Vector<TType, C_NB_DIM>  result;

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				result.values[i] = values[i] + p_other.values[i];
			}

			return (result);
		}

		Vector<TType, C_NB_DIM> operator - (const Vector<TType, C_NB_DIM>& p_other) const
		{
			Vector<TType, C_NB_DIM>  result;

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				result.values[i] = values[i] - p_other.values[i];
			}

			return (result);
		}

		Vector<TType, C_NB_DIM> operator * (const Vector<TType, C_NB_DIM>& p_other) const
		{
			Vector<TType, C_NB_DIM>  result;

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				result.values[i] = values[i] * p_other.values[i];
			}

			return (result);
		}

		Vector<TType, C_NB_DIM> operator / (const Vector<TType, C_NB_DIM>& p_other) const
		{
			Vector<TType, C_NB_DIM>  result;

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				if (p_other.values[i] == 0)
					throw jgl::Exception("Try to divide by 0");
				result.values[i] = values[i] / p_other.values[i];
			}

			return (result);
		}

		Vector<TType, C_NB_DIM>& operator += (const Vector<TType, C_NB_DIM>& p_other)
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				values[i] = values[i] + p_other.values[i];
			}

			return (*this);
		}

		Vector<TType, C_NB_DIM>& operator -= (const Vector<TType, C_NB_DIM>& p_other)
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				values[i] = values[i] - p_other.values[i];
			}

			return (*this);
		}

		Vector<TType, C_NB_DIM>& operator *= (const Vector<TType, C_NB_DIM>& p_other)
		{
			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				values[i] = values[i] * p_other.values[i];
			}

			return (*this);
		}

		Vector<TType, C_NB_DIM>& operator /= (const Vector<TType, C_NB_DIM>& p_other)
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
				throw jgl::Exception("Try to divide by 0");

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

		jgl::String text() const
		{
			jgl::String result = "";

			for (jgl::Size_t i = 0; i < C_NB_DIM; i++)
			{
				if (i != 0)
					result += " / ";
				jgl::String tmp = std::to_string(values[i]);
				result += tmp;
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
		TType& x;
		TType& y;

		BaseVector2() : Vector<TType, 2>(), x(this->values[0]), y(this->values[1])
		{

		}
		BaseVector2(TType p_x, TType p_y) : x(this->values[0]), y(this->values[1])
		{
			this->values[0] = p_x;
			this->values[1] = p_y;
		}

		BaseVector2<TType> operator = (const BaseVector2<TType>& p_other)
		{
			BaseVector2<TType> result;

			for (jgl::Size_t i = 0; i < 2; i++)
			{
				result.values[i] = p_other.values[i];
			}

			return (result);
		}
	};

	template <typename TType>
	struct BaseVector3 : public Vector<TType, 3>
	{
		TType& x;
		TType& y;
		TType& z;

		BaseVector3() : Vector<TType, 3>(), x(this->values[0]), y(this->values[1]), z(this->values[2])
		{

		}
		BaseVector3(TType p_x, TType p_y, TType p_z) : x(this->values[0]), y(this->values[1]), z(this->values[2])
		{
			this->values[0] = p_x;
			this->values[1] = p_y;
			this->values[2] = p_z;
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
		TType& x;
		TType& y;
		TType& z;
		TType& w;

		BaseVector4() : Vector<TType, 4>(), x(this->values[0]), y(this->values[1]), z(this->values[2]), w(this->values[3])
		{

		}
		BaseVector4(TType p_x, TType p_y, TType p_z, TType p_w) : x(this->values[0]), y(this->values[1]), z(this->values[2]), w(this->values[3])
		{
			this->values[0] = p_x;
			this->values[1] = p_y;
			this->values[2] = p_z;
			this->values[3] = p_w;
		}

		BaseVector4<TType>& operator = (const BaseVector4<TType>& p_other)
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
}