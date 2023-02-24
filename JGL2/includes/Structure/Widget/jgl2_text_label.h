#pragma once

#include "jgl2_widget_core.h"
#include "Component\jgl2_box_component.h"
#include "Component\jgl2_label_component.h"

namespace jgl
{
	class Label : public jgl::WidgetCore
	{
	private:
		jgl::WidgetComponent::Box _box;
		jgl::WidgetComponent::Label _label;

		jgl::Bool _needLabelTextSizeCalculation = false;

		jgl::Bool _onUpdate()
		{
			return (false);
		}
		void _onRender()
		{
			_label.render();
			_box.render();
		}
		void _onGeometryChange()
		{
			_box.setGeometry(anchor(), size());
			_box.setDepth(depth());

			_label.setGeometry(_box.usableAnchor(), _box.usableSize());
			_label.setDepth(depth() + 0.5f);
		}

	public:
		Label(std::string p_name) : jgl::WidgetCore(p_name),
			_needLabelTextSizeCalculation(false)
		{
			_label.setText("");
		}

		jgl::WidgetComponent::Box& box() { return (_box); }
		jgl::WidgetComponent::Label& label() { return (_label); }

		const jgl::WidgetComponent::Box& box() const { return (_box); }
		const jgl::WidgetComponent::Label& label() const { return (_label); }
	};
}