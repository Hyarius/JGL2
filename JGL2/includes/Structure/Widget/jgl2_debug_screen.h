#pragma once

#include "Structure/Widget/jgl2_widget_core.h"
#include "Structure/Widget/Component/jgl2_label_component.h"

namespace jgl
{
	namespace Widget
	{
		template <jgl::Size_t C_NB_COLLUMN, jgl::Size_t C_NB_ROW>
		class DebugScreen : public jgl::Abstract::Widget::Core
		{
		private:
			jgl::Widget::Component::Label::DefaultValues _defaultLabelValues;
			jgl::Widget::Component::Label _labels[C_NB_COLLUMN][C_NB_ROW];

			jgl::Bool _onUpdate()
			{
				return (false);
			}

			void _computeTextSize()
			{
				jgl::Size_t newSize = UINT32_MAX;

				for (jgl::Size_t i = 0; i < C_NB_COLLUMN; i++)
				{
					for (jgl::Size_t j = 0; j < C_NB_ROW; j++)
					{
						jgl::Size_t tmpTextSize = _labels[i][j].calculateTextSize();

						if (tmpTextSize < newSize)
							newSize = tmpTextSize;
					}
				}
				if (newSize != UINT32_MAX)
				{
					_defaultLabelValues.textSize = newSize;
				}
			}

			void _onGeometryChange()
			{
				jgl::Vector2Int labelSize = size() / jgl::Vector2Int(C_NB_COLLUMN, C_NB_ROW);
				for (jgl::Size_t i = 0; i < C_NB_COLLUMN; i++)
				{
					for (jgl::Size_t j = 0; j < C_NB_ROW; j++)
					{
						_labels[i][j].setGeometry(jgl::Vector2Int(i, j) * labelSize, labelSize);
					}
				}
				_computeTextSize();
			}

			void _onRender()
			{
				for (jgl::Size_t i = 0; i < C_NB_COLLUMN; i++)
				{
					for (jgl::Size_t j = 0; j < C_NB_ROW; j++)
					{
						_labels[i][j].render();
					}
				}
			}

		public:
			DebugScreen(std::string p_name) : jgl::Abstract::Widget::Core(p_name)
			{
				for (jgl::Size_t i = 0; i < C_NB_COLLUMN; i++)
				{
					for (jgl::Size_t j = 0; j < C_NB_ROW; j++)
					{
						_labels[i][j].setDefaultValues(_defaultLabelValues);
					
						if (i == 0)
						{
							_labels[i][j].setHorizontalAlignment(jgl::HorizontalAlignment::Left);
						}
						else if (i == C_NB_COLLUMN - 1)
						{
							_labels[i][j].setHorizontalAlignment(jgl::HorizontalAlignment::Right);
						}
					}
				}
			}

			void setText(std::string p_text, jgl::Size_t p_collumn, jgl::Size_t p_row)
			{
				_labels[p_collumn][p_row].setText(p_text);
				_computeTextSize();
			}
		};
	}
}