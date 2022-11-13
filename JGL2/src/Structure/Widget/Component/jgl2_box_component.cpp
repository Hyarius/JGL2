#include "Structure/Widget/Component/jgl2_box_component.h"
#include "jgl2_drawing_functions.h"
#include "Structure/Widget/jgl2_widget.h"

namespace jgl
{
	namespace WidgetComponent
	{
		Box::Box(Widget* p_owner) :
			_owner(p_owner)
		{

		}

		const Widget* Box::owner() const
		{
			return (_owner);
		}

		const Color& Box::backgroundColor() const
		{
			return (_backgroundColor);
		}

		const Color& Box::frontgroundColor() const
		{
			return (_frontgroundColor);
		}

		const Vector2Int& Box::anchor() const
		{
			return (_anchor);
		}

		const Vector2Int& Box::size() const
		{
			return (_size);
		}

		const Vector2Int& Box::borderSize() const
		{
			return (_borderSize);
		}

		const Vector2Int& Box::usableAnchor() const
		{
			return (_usableAnchor);
		}
		
		const Vector2Int& Box::usableSize() const
		{
			return (_usableSize);
		}

		void Box::render()
		{
			drawRectangleColor(_backgroundColor, _anchor, _size, _owner->depth());
			drawRectangleColor(_frontgroundColor, _anchor + _borderSize, _size - _borderSize * jgl::Vector2Int(2, 2), _owner->depth() + 0.1f);
		}

		void Box::setColor(Color p_backgroundColor, Color p_frontgroundColor)
		{
			_frontgroundColor = p_frontgroundColor;
			_backgroundColor = p_backgroundColor;
		}

		void Box::setGeometry(Vector2Int p_anchor, Vector2Int p_size)
		{
			_anchor = p_anchor;
			_size = p_size;
			_usableAnchor = _anchor + _borderSize;
			_usableSize = _size - _borderSize * Vector2Int(2, 2);
		}

		void Box::setBorderSize(jgl::Vector2Int p_borderSize)
		{
			_borderSize = p_borderSize;
			_usableAnchor = _anchor + _borderSize;
			_usableSize = _size - _borderSize * Vector2Int(2, 2);
		}
	}
}