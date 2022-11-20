#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "Structure/jgl2_vector.h"

namespace jgl
{
	class Polygon2D
	{
		friend class Polygon2D;

	private:
		struct SegmentData
		{
			jgl::Size_t a;
			jgl::Size_t b;
			jgl::Vector2 norm;
		};

		jgl::Vector2 _center;
		jgl::Vector2 _size = jgl::Vector2(1, 1);
		std::vector<jgl::Vector2> _points;
		std::vector<SegmentData> _segments;

		struct AxisProjection
		{
			jgl::Float min;
			jgl::Float max;
		};

		AxisProjection _projectPointOnAxis(jgl::Vector2 p_point, jgl::Vector2 p_axis, jgl::Vector2 p_deltaPos = jgl::Vector2(0, 0)) const
		{
			AxisProjection result;

			jgl::Float tmp_dot = p_axis.dot(p_point + p_deltaPos);
			result.min = tmp_dot;
			result.max = tmp_dot;

			return (result);
		}

		AxisProjection _projectionOnAxis(jgl::Vector2 p_axis, jgl::Vector2 p_deltaPos = jgl::Vector2(0, 0)) const
		{
			AxisProjection result;

			jgl::Float tmp_dot = p_axis.dot(_points[0] * _size + _center + p_deltaPos);
			result.min = tmp_dot;
			result.max = tmp_dot;

			for (jgl::Size_t i = 1; i < _points.size(); i++)
			{
				tmp_dot = p_axis.dot(_points[i] * _size + _center + p_deltaPos);
				if (tmp_dot < result.min)
					result.min = tmp_dot;
				else if (tmp_dot > result.max)
					result.max = tmp_dot;
			}

			return (result);
		}

		jgl::Float _calcDistance(AxisProjection p_a, AxisProjection p_b) const
		{
			if (p_a.min < p_b.min)
				return p_b.min - p_a.max;
			else
				return p_a.min - p_b.max;
		}

	public:
		void setCenter(jgl::Vector2 p_center)
		{
			_center = p_center;
		}
		void setSize(jgl::Vector2 p_size)
		{
			_size = p_size;
		}
		void addPoint(jgl::Vector2 p_point)
		{
			_points.push_back(p_point);
		}
		void addSegment(jgl::Size_t p_a, jgl::Size_t p_b)
		{
			SegmentData result;

			result.a = p_a;
			result.b = p_b;
			result.norm = _points[p_a].cross(_points[p_b]).normalize();

			_segments.push_back(result);
		}

		void resetPoints()
		{
			_points.clear();
		}
		void resetIndexes()
		{
			_segments.clear();
		}

		jgl::Bool collideWithPolygon(const Polygon2D& p_other, jgl::Vector2 p_deltaPos = jgl::Vector2(0, 0))
		{
			jgl::Bool result = true;

			for (jgl::Size_t i = 0; i < _segments.size(); i++)
			{
				AxisProjection projectionA = _projectionOnAxis(_segments[i].norm);
				AxisProjection projectionB = p_other._projectionOnAxis(_segments[i].norm, p_deltaPos);

				if (_calcDistance(projectionA, projectionB) > 0)
					result = false;
			}

			return (result);
		}

		jgl::Bool collideWithPoint(const jgl::Vector2& p_point, jgl::Vector2 p_deltaPos = jgl::Vector2(0, 0))
		{
			jgl::Bool result = true;

			for (jgl::Size_t i = 0; i < _segments.size(); i++)
			{
				AxisProjection projectionA = _projectionOnAxis(_segments[i].norm);
				AxisProjection projectionB = _projectPointOnAxis(p_point, _segments[i].norm, p_deltaPos);

				if (_calcDistance(projectionA, projectionB) > 0)
					result = false;
			}

			return (result);
		}
	};
}