#include "Structure/Widget/Component/jgl2_box_component.h"
#include "jgl2_drawing_functions.h"
#include "Structure/jgl2_application.h"

namespace jgl
{
	namespace WidgetComponent
	{
		Box::Box()
		{

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

		void Box::render(jgl::Float p_depthDelta)
		{
			drawRectangleColor(_backgroundColor, _anchor, _size, p_depthDelta);
			drawRectangleColor(_frontgroundColor, _anchor + _borderSize, _size - _borderSize * jgl::Vector2Int(2, 2), p_depthDelta + 0.1f);
		}

		Bool Box::isPointed() const
		{
			Vector2Int pos = jgl::Application::instance()->mouse().pos();

			if (pos.x() < anchor().x() || pos.x() >= anchor().x() + size().x() ||
				pos.y() < anchor().y() || pos.y() >= anchor().y() + size().y())
				return (false);

			return (true);
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