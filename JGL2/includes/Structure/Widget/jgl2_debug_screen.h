#pragma once

#include "Structure/Widget/jgl2_widget_core.h"
#include "Structure/Widget/Component/jgl2_label_component.h"

namespace jgl
{
	namespace Widget
	{
		template <const jgl::Size_t C_NB_COLLUMNS, const jgl::Size_t C_NB_ROWS>
		class DebugScreen : public jgl::Abstract::Widget::Core
		{
		private:
			jgl::Widget::Component::Label::DefaultValues _labelsDefaultValues;
			jgl::Widget::Component::Label _labels[C_NB_COLLUMNS][C_NB_ROWS];

			jgl::Bool _onUpdate()
			{
				return (false);
			}

			void _computeLabelsTextSize()
			{
				jgl::Size_t newTextSize = UINT32_MAX;

				for (jgl::Size_t i = 0; i < C_NB_COLLUMNS; i++)
				{
					for (jgl::Size_t j = 0; j < C_NB_ROWS; j++)
					{
						jgl::Size_t tmpTextSize = _labels[i][j].calculateTextSize();
						if (tmpTextSize < newTextSize)
						{
							newTextSize = tmpTextSize;
						}
					}
				}
				_labelsDefaultValues.textSize = newTextSize;
			}

			void _onGeometryChange()
			{
				jgl::Vector2Int labelSize = (size() - 20) / jgl::Vector2Int(C_NB_COLLUMNS, C_NB_ROWS);
				for (jgl::Size_t i = 0; i < C_NB_COLLUMNS; i++)
				{
					for (jgl::Size_t j = 0; j < C_NB_ROWS; j++)
					{
						_labels[i][j].setGeometry(jgl::Vector2Int(i, j) * labelSize + 10, labelSize);
					}
				}
				_computeLabelsTextSize();
			}

			void _onRender()
			{
				for (jgl::Size_t i = 0; i < C_NB_COLLUMNS; i++)
				{
					for (jgl::Size_t j = 0; j < C_NB_ROWS; j++)
					{
						_labels[i][j].render();
					}
				}
			}

		public:
			DebugScreen(std::string p_name) : jgl::Abstract::Widget::Core(p_name)
			{
				_labelsDefaultValues = jgl::Widget::Component::Label::defaultValues;
				for (jgl::Size_t i = 0; i < C_NB_COLLUMNS; i++)
				{
					for (jgl::Size_t j = 0; j < C_NB_ROWS; j++)
					{
						_labels[i][j].setDefaultValues(_labelsDefaultValues);
						if (i == 0)
							_labels[i][j].setHorizontalAlignment(jgl::HorizontalAlignment::Left);
						else if (i == C_NB_COLLUMNS - 1)
							_labels[i][j].setHorizontalAlignment(jgl::HorizontalAlignment::Right);
					}
				}
			}

			void setText(std::string p_text, jgl::Size_t p_positionX, jgl::Size_t p_positionY)
			{
				_labels[p_positionX][p_positionY].setText(p_text);
			}
		};
	}
}