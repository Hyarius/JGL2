#pragma once

#include "jgl2_basic_types.h"
#include "Structure/Widget/jgl2_widget.h"

namespace jgl
{
	class TextEntry : public jgl::Widget
	{
	private:
		jgl::WidgetComponent::Box _box;
		jgl::WidgetComponent::TextLabel _label;

		jgl::Bool _selected = false;

		std::string _entry;

		jgl::Bool _masked = false;

		jgl::Size_t _cursor = 0;
		jgl::Size_t _lowerCursor = 0;
		jgl::Size_t _higherCursor = 0;
		
		jgl::Color _cursorColor = jgl::Color(0, 0, 0);
		jgl::Vector2Int _cursorPosition;
		jgl::Vector2Int _cursorSize;

		jgl::Bool _onUpdate();
		void _onRender();
		void _onGeometryChange();

		void _addCharInEntry(jgl::Char p_char);
		void _deleteCharInEntry(jgl::Int p_cursorPositionDelta);
		void _moveCursor(jgl::Int p_cursorPositionDelta);

		void _computeHigherCursor();
		void _moveHigherCursor();
		void _moveLowerCursor();

		void _computeCursorPosition();

		std::string _computeTextToRender();

	public:
		TextEntry(jgl::Widget* p_parent);

		void setCursorColor(jgl::Color p_color);
		const jgl::Color& cursorColor() const;

		void maskText();
		void unmaskText();
		void setMaskedText(jgl::Bool p_status);
		const jgl::Bool& maskedText() const;

		void select();
		void unselect();
		void setSelectStatus(jgl::Bool p_status);
		jgl::Bool isSelected();
	};
}