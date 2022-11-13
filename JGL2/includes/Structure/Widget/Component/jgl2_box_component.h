#pragma once

#include "Structure/jgl2_vector.h"
#include "Structure/jgl2_color.h"

namespace jgl
{
	namespace WidgetComponent
	{
		class Box
		{
		private:
			Color _backgroundColor = Color(120, 120, 120);
			Color _frontgroundColor = Color(150, 150, 150);

			Vector2Int _anchor = Vector2Int(0, 0);
			Vector2Int _size = Vector2Int(0, 0);
			Vector2Int _usableAnchor = Vector2Int(0, 0);
			Vector2Int _usableSize = Vector2Int(0, 0);
			Vector2Int _borderSize = Vector2Int(0, 0);

		public:
			Box();
			void render(jgl::Float p_depthDelta);

			void setColor(Color p_backgroundColor, Color p_frontgroundColor);
			void setGeometry(Vector2Int p_anchor, Vector2Int p_size);
			void setBorderSize(jgl::Vector2Int p_borderSize);

			const Color& backgroundColor() const;
			const Color& frontgroundColor() const;
			const Vector2Int& anchor() const;
			const Vector2Int& size() const;
			const Vector2Int& borderSize() const;

			const Vector2Int& usableAnchor() const;
			const Vector2Int& usableSize() const;
		};
	}
}