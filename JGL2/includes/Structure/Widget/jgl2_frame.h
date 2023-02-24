#pragma once

#include "jgl2_widget_core.h"
#include "Structure/Widget/Component/jgl2_box_component.h"

namespace jgl
{
	class Frame : public jgl::WidgetCore
	{
	private:
		jgl::WidgetComponent::Box _box;

		jgl::Bool _onUpdate()
		{
			return (false);
		}
		void _onRender()
		{
			_box.render();
		}
		void _onGeometryChange()
		{
			_box.setGeometry(anchor(), size());
			_box.setDepth(depth());
		}

	public:
		Frame(std::string p_name) : jgl::WidgetCore(p_name)
		{
			viewport()->setOffset(5, 10);
		}

		jgl::WidgetComponent::Box& box() { return (_box); }
		const jgl::WidgetComponent::Box& box() const { return (_box); }
	};
}