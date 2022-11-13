#pragma once

#include "Structure/jgl2_color.h"
#include "Structure/Widget/jgl2_widget.h"
#include "Structure/Widget/Component/jgl2_box_component.h"

namespace jgl
{
	class Frame : public jgl::Widget
	{
	private:
		WidgetComponent::Box _box;

		jgl::Bool _onUpdate();
		void _onRender();
		void _onGeometryChange();

	public:
		Frame(jgl::Widget* p_parent);
		void setColor(jgl::Color p_backgroundColor, jgl::Color p_frontgroundColor);
	};
}