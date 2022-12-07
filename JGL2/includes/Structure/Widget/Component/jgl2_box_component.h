#pragma once

#include "Structure/jgl2_vector.h"
#include "Structure/jgl2_color.h"
#include "Structure/jgl2_shader.h"

namespace jgl
{
	namespace WidgetComponent
	{
		class Box
		{
		private:
			Bool _initialized = false;
			Shader* _shader = nullptr;
			Buffer* _modelBuffer = nullptr;
			Buffer* _colorBuffer = nullptr;
			Buffer* _elementBuffer = nullptr;
			Bool _computed = false;
			
			Color _backgroundColor = Color(120, 120, 120);
			Color _frontgroundColor = Color(150, 150, 150);

			Vector2Int _anchor = Vector2Int(0, 0);
			Vector2Int _size = Vector2Int(0, 0);
			Vector2Int _usableAnchor = Vector2Int(0, 0);
			Vector2Int _usableSize = Vector2Int(0, 0);
			Vector2Int _borderSize = Vector2Int(5, 5);

			void _initiatizeOpenGL();
			void _computeShaderBuffer(Float p_depth);
			void _castRender();

		public:
			Box();
			void render(Float p_depth);
			Bool isPointed() const;

			void setColor(Color p_backgroundColor, Color p_frontgroundColor);
			void setGeometry(Vector2Int p_anchor, Vector2Int p_size);
			void setBorderSize(Vector2Int p_borderSize);

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