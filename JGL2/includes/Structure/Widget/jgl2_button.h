#pragma once

#include "jgl2_basic_types.h"
#include "jgl2_includes.h"
#include "Structure/Widget/jgl2_widget.h"

namespace jgl
{
	class Button : public jgl::Widget
	{
	private:
		jgl::Size_t _definedTextSize = 0;
		std::function< void() > _funct;

		jgl::Bool _pressed;
		jgl::WidgetComponent::Box _boxes[2];
		jgl::WidgetComponent::TextLabel _labels[2];

		jgl::Bool _onUpdate();
		void _onRender();
		void _onGeometryChange();

	public:
		Button(std::function< void() > p_funct, jgl::Widget* p_parent);

		void setDefinedTextSize(const jgl::Size_t& p_definedTextSize);
		const jgl::Size_t definedTextSize();

		void setFunct(const std::function<void()>& p_funct);
		const std::function<void()>& funct();

		void setText(std::string p_text);

		jgl::WidgetComponent::Box* boxes();
		jgl::WidgetComponent::Box& selectedBox();
		jgl::WidgetComponent::Box& unselectedBox();
		jgl::WidgetComponent::TextLabel* labels();
		jgl::WidgetComponent::TextLabel& selectedTextLabel();
		jgl::WidgetComponent::TextLabel& unselectedTextLabel();
	};
}