#pragma once
#include "structure/jgl2_vector.h"

namespace jgl
{
	struct Segment
	{
		jgl::Uint a;
		jgl::Uint b;
		Segment()
		{
			a = 0;
			b = 0;
		}
		Segment(jgl::Uint p_a, jgl::Uint p_b)
		{
			a = p_a;
			b = p_b;
		}
		friend std::ostream& operator<<(std::ostream& os, const jgl::Segment value)
		{
			os << value.a << " - " << value.b;
			return os;
		}
	};
	class Shape {
	private:
		jgl::Array<jgl::Vector2Int> _points;
		jgl::Array<jgl::Segment> _segments;
	public:
		Shape();
		Shape(const Shape& other);
		Shape(Shape& other);
		const Shape& operator = (const Shape& other);

		const jgl::Array<jgl::Vector2Int>& points() const { return (_points); }
		const jgl::Array<jgl::Segment>& segments() const { return (_segments); }

		void add_point(jgl::Vector2Int to_add);
		void add_point(jgl::Int p_a, jgl::Int p_b);
		void add_segment(jgl::Segment to_add);
		void add_segment(jgl::Uint p_a, jgl::Uint p_b);
		void clear() { _points.clear(); _segments.clear(); }
		friend std::ostream& operator<<(std::ostream& os, const jgl::Shape value)
		{
			os << "Points [" << value.points().size() << "] : " << value.points() << std::endl;
			os << "Value [" << value.segments().size() << "] : " << value.segments() << std::endl;
			return os;
		}
	};
}