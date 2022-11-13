#include "Structure/Widget/Component/jgl2_text_label_component.h"

namespace jgl
{
	namespace WidgetComponent
	{
		TextLabel::TextLabel(Widget* p_owner) : _owner(p_owner)
		{

		}

		void TextLabel::render()
		{

		}

		void TextLabel::setColor(Color p_textColor, Color p_outlineColor)
		{
			_textColor = p_textColor;
			_outlineColor = p_outlineColor;
		}
		
		void TextLabel::setGeometry(Vector2Int p_anchor, Vector2Int p_size)
		{
			_anchor = p_anchor;
			_size = p_size;
		}

		const Color& TextLabel::textColor() const
		{
			return (_textColor);
		}

		const Color& TextLabel::outlineColor() const
		{
			return (_outlineColor);
		}

		const Vector2Int& TextLabel::anchor() const
		{
			return (_anchor);
		}

		const Vector2Int& TextLabel::size() const
		{
			return (_size);
		}
	}
}
