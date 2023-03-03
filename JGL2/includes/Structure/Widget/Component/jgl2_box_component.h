#pragma once

#include "Structure/jgl2_color.h"
#include "Structure/jgl2_vector2.h"
#include "Structure/jgl2_shader.h"

#include "Structure/jgl2_monitored_value.h"

#include "Structure/Application/Graphical/jgl2_graphical_application.h"

namespace jgl
{
	namespace Widget
	{
		namespace Component
		{
			class Box
			{
			public:
				struct DefaultValues
				{
					jgl::Color borderColor = jgl::Color(120, 120, 120);
					jgl::Color color = jgl::Color(150, 150, 150);
					jgl::Vector2Int borderSize = jgl::Vector2Int(5, 5);
				};
				static inline DefaultValues defaultValues;

			private:
				jgl::Bool _shaderInitialized = false;
				jgl::Shader* _shader = nullptr;
				jgl::Buffer* _modelSpace = nullptr;
				jgl::Buffer* _modelDepth = nullptr;
				jgl::Buffer* _modelColor = nullptr;
				jgl::Buffer* _elementBuffer = nullptr;

				static inline jgl::Vector2 _modelSpaceBufferData[8];
				static inline jgl::Float _modelDepthBufferData[8];
				static inline jgl::Color _modelColorBufferData[8];
				static inline jgl::UInt _elementBufferData[12] = { 0, 3, 1, 2, 3, 0, 4, 7, 5, 6, 7, 4 };

				MonitoredValue<jgl::Color> _color;
				MonitoredValue<jgl::Color> _borderColor;
				MonitoredValue<jgl::Vector2Int> _borderSize;

				jgl::Vector2Int _anchor;
				jgl::Vector2Int _size;
				jgl::Float _depth;

				jgl::Bool _colorBaked = false;
				jgl::Bool _verticesBaked = false;
				jgl::Bool _depthBaked = false;

				void _initializeShaderDatas();
				void _bakeColors();
				void _bakeVertices();
				void _bakeDepth();

			public:
				Box();

				void setDefaultValues(DefaultValues& p_defaultValue);

				void reset();
				void useValue();

				void setBorderSize(jgl::Vector2Int p_borderSize);
				void setColors(jgl::Color p_color, jgl::Color p_borderColor);
				void setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size);
				void setDepth(jgl::Float p_depth);
				void render();

				const jgl::Color& color() const { return(_color.value()); }
				const jgl::Color& borderColor() const { return(_borderColor.value()); }
				const jgl::Vector2Int& borderSize() const { return(_borderSize.value()); }
				const jgl::Vector2Int& anchor() const { return (_anchor); }
				const jgl::Vector2Int& size() const { return (_size); }
				jgl::Vector2Int usableAnchor() const { return (_anchor + borderSize()); }
				jgl::Vector2Int usableSize() const { return (_size - borderSize() * 2); }
			};
		}
	}
}