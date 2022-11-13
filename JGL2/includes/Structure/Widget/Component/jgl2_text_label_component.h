#pragma once

#include "Structure/jgl2_vector.h"
#include "Structure/jgl2_color.h"

namespace jgl
{
	class Widget;

	namespace WidgetComponent
	{
		class TextLabel
		{
		private:
			Widget* _owner;
			std::string _text;
			Color _textColor;
			Color _outlineColor;

			Vector2Int _anchor = Vector2Int(0, 0);
			Vector2Int _size = Vector2Int(0, 0);

			Vector2Int _textAnchor = Vector2Int(0, 0);
		public:
			TextLabel(Widget* p_owner);

			void render();
			void setColor(Color p_textColor, Color p_outlineColor);
			void setGeometry(Vector2Int p_anchor, Vector2Int p_size);

			const Color& textColor() const;
			const Color& outlineColor() const;
			const Vector2Int& anchor() const;
			const Vector2Int& size() const;
		};
	}
}