#include "jgl2.h"

namespace jgl
{
	Shape::Shape()
	{

	}
	Shape::Shape(const Shape& other)
	{
		std::cout << "Calling const copy constructor" << std::endl;
		for (jgl::Size_t i = 0; i < other.points().size(); i++)
			add_point(other.points()[i]);
		for (jgl::Size_t i = 0; i < other.segments().size(); i++)
			add_segment(other.segments()[i]);
		std::cout << "In const copy constructor : " << _points.size() << " vs " << other.points().size() << std::endl;
	}
	Shape::Shape(Shape& other)
	{
		std::cout << "Calling copy constructor" << std::endl;
		for (jgl::Size_t i = 0; i < other.points().size(); i++)
			add_point(other.points()[i]);
		for (jgl::Size_t i = 0; i < other.segments().size(); i++)
			add_segment(other.segments()[i]);
		std::cout << "In copy constructor : " << _points.size() << " vs " << other.points().size() << std::endl;
	}
	const Shape& Shape::operator = (const Shape& other)
	{
		this->clear();
		std::cout << "Assignement operator" << std::endl;
		this->_points = other.points();
		this->_segments = other.segments();
		std::cout << "In assignement operator : " << _points.size() << " vs " << other.points().size() << std::endl;
		return (*this);
	}
	void Shape::add_point(jgl::Vector2Int to_add)
	{
		_points.push_back(to_add);
	}
	void Shape::add_point(jgl::Int p_a, jgl::Int p_b)
	{
		_points.push_back(jgl::Vector2Int(p_a, p_b));
	}
	void Shape::add_segment(jgl::Segment to_add)
	{
		_segments.push_back(to_add);
	}
	void Shape::add_segment(jgl::Uint p_a, jgl::Uint p_b)
	{
		_segments.push_back(Segment(p_a, p_b));
	}
}