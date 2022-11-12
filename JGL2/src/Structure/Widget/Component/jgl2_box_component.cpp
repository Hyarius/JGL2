#include "Structure/Widget/Component/jgl2_box_component.h"
#include "jgl2_drawing_functions.h"
#include "Structure/Widget/jgl2_widget.h"

namespace jgl
{
	namespace WidgetComponent
	{
		BoxComponent::BoxComponent(Widget* p_owner) :
			_owner(p_owner)
		{

		}

		const Widget* BoxComponent::owner() const
		{
			return (_owner);
		}

		const Color& BoxComponent::backgroundColor() const
		{
			return (_backgroundColor);
		}

		const Color& BoxComponent::frontgroundColor() const
		{
			return (_frontgroundColor);
		}

		const Vector2Int& BoxComponent::anchor() const
		{
			return (_anchor);
		}

		const Vector2Int& BoxComponent::size() const
		{
			return (_size);
		}

		const Vector2Int& BoxComponent::borderSize() const
		{
			return (_borderSize);
		}

		void BoxComponent::render()
		{
			drawRectangleColor(_backgroundColor, Vector2Int(0, 0), _size, _owner->depth());
			drawRectangleColor(_frontgroundColor, _borderSize, _size - _borderSize * jgl::Vector2Int(2, 2), _owner->depth() + 0.1f);
		}

		void BoxComponent::setColor(Color p_backgroundColor, Color p_frontgroundColor)
		{
			_frontgroundColor = p_frontgroundColor;
			_backgroundColor = p_backgroundColor;
		}

		void BoxComponent::setGeometry(Vector2Int p_anchor, Vector2Int p_size)
		{
			_anchor = p_anchor;
			_size = p_size;
		}

		void BoxComponent::setBorderSize(jgl::Vector2Int p_borderSize)
		{
			_borderSize = p_borderSize;
		}
	}
}