#pragma once

#include "Structure/Widget/jgl2_widget.h"
#include "Structure/Widget/Component/jgl2_box_component.h"
#include "Structure/Widget/Component/jgl2_text_label_component.h"

namespace jgl
{
	class TextLabel : public jgl::Widget
	{
	private:
		jgl::Size_t _definedTextSize = 0;
		jgl::WidgetComponent::Box _box;
		jgl::WidgetComponent::TextLabel _label;

		jgl::Bool _onUpdate();
		void _onGeometryChange();
		void _onRender();
		void _onPositionChange();

	public:
		TextLabel(std::string p_text, jgl::Widget* p_parent);
		
		void setDefinedTextSize(const jgl::Size_t& p_definedTextSize);
		const jgl::Size_t& definedTextSize();

		jgl::WidgetComponent::Box& box();
		jgl::WidgetComponent::TextLabel& label();
	};
}