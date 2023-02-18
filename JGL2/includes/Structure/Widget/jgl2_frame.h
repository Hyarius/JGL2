#pragma once

#include "jgl2_widget_core.h"
#include "Structure/Widget/Component/jgl2_box_component.h"

namespace jgl
{
	class Frame : public jgl::WidgetCore
	{
	private:
		Box _box;

		jgl::Bool _onUpdate();
		void _onRender();
		void _onGeometryChange();

	public:
		Frame(std::string p_name);
		Box& box() { return (_box); }
	};
}