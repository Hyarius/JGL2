#pragma once

#include "jgl2_basic_types.h"
#include "Structure/Widget/jgl2_widget.h"

namespace jgl
{
	class TextEntry : public jgl::Widget
	{
	private:
		jgl::Size_t _definedTextSize = 0;
		jgl::WidgetComponent::Box _box;
		jgl::WidgetComponent::TextLabel _label;

		jgl::Bool _selected = false;

		jgl::Size_t _cursor = 0;
		jgl::Color _cursorColor = jgl::Color(0, 0, 0);
		jgl::Vector2Int _cursorPosition = jgl::Vector2Int(0, 0);
		jgl::Vector2Int _cursorSize = jgl::Vector2Int(3, 10);

		std::string _totalText;

		jgl::Bool _onUpdate();
		void _onRender();
		void _onGeometryChange();

		void _computeCursorPosition();
		std::string _computeRenderableText();
		void _updateText(std::string p_totalText);

	public:
		TextEntry(jgl::Widget* p_parent);
		void setDefinedTextSize(const jgl::Size_t& p_definedTextSize);
		const jgl::Size_t definedTextSize();

		void setCursorColor(const jgl::Color& p_cursorColor);
		const jgl::Color cursorColor();

		void select();
		void unselect();
		void setSelectStatus(jgl::Bool p_status);
		jgl::Bool isSelected();
	};
}