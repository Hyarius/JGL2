#pragma once

#include "Structure/jgl2_color.h"
#include "Structure/Widget/jgl2_widget.h"

namespace jgl
{
	class Frame : public jgl::Widget
	{
	private:
		jgl::Color _backgroundColor = jgl::Color(120, 120, 120);
		jgl::Color _frontgroundColor = jgl::Color(150, 150, 150);

		jgl::Bool _onUpdate();
		void _onRender();
		void _onGeometryChange();

	public:
		Frame(jgl::Widget* p_parent);
		void setColor(jgl::Color p_backgroundColor, jgl::Color p_frontgroundColor);
	};
}