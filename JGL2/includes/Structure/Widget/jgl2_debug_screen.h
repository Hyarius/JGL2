#pragma once

#include "jgl2_includes.h"
#include "Structure/jgl2_vector.h"
#include "Structure/jgl2_timer.h"
#include "Structure/Widget/jgl2_text_label.h"

namespace jgl
{
	template <const jgl::Size_t NumberCollumn, const jgl::Size_t NumberLine>
	class DebugScreen : public jgl::Widget
	{
	public:
		static const jgl::Size_t C_NB_LINES = NumberLine;
		static const jgl::Size_t C_NB_COLLUMNS = NumberCollumn;

	private:

		jgl::TextLabel* _lines[C_NB_LINES][C_NB_COLLUMNS];

		void _onRender()
		{

		}
		void _onGeometryChange()
		{
			jgl::Float unitValue = size().y() / static_cast<jgl::Float>(C_NB_LINES);
			jgl::Vector2 unit = jgl::Vector2Int(unitValue, unitValue);

			jgl::Vector2 labelSize = jgl::Vector2((size().x()) / static_cast<jgl::Float>(C_NB_COLLUMNS), unit.y());

			for (jgl::Size_t i = 0; i < C_NB_LINES; i++)
			{
				for (jgl::Size_t j = 0; j < C_NB_COLLUMNS; j++)
				{
					_lines[i][j]->setGeometry(labelSize * jgl::Vector2(j, i), labelSize);
				}
			}
		}

		jgl::Bool _onUpdate()
		{
			return (false);
		}

		void _onPositionChange()
		{

		}

	public:
		DebugScreen(jgl::Widget* p_parent) : jgl::Widget(p_parent)
		{
			for (jgl::Size_t i = 0; i < C_NB_LINES; i++)
			{
				for (jgl::Size_t j = 0; j < C_NB_COLLUMNS; j++)
				{
					_lines[i][j] = new jgl::TextLabel("", this);
					_lines[i][j]->box().setColor(jgl::Color(0, 0, 0, 0), jgl::Color(0, 0, 0, 0));
					_lines[i][j]->label().setColor(jgl::Color(255, 255, 255), jgl::Color(0, 0, 0));
					if (j == C_NB_COLLUMNS - 1)
						_lines[i][j]->label().setHorizontalAlignment(jgl::HorizontalAlignment::Right);
					else if (j == 0)
						_lines[i][j]->label().setHorizontalAlignment(jgl::HorizontalAlignment::Left);
					else
						_lines[i][j]->label().setHorizontalAlignment(jgl::HorizontalAlignment::Centred);

					_lines[i][j]->label().setVerticalAlignment(jgl::VerticalAlignment::Centred);

					_lines[i][j]->activate();
				}
			}
		}

		void setText(std::string p_text, jgl::Size_t p_collumns, jgl::Size_t p_line)
		{
			if (p_line >= C_NB_LINES || p_collumns >= C_NB_COLLUMNS)
				throw std::runtime_error("Debug_line out of range");
			_lines[p_line][p_collumns]->label().setText(p_text);
		}
	};
}