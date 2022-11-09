#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "Structure/jgl2_vector.h"

namespace jgl
{
	class Application;

	class Widget
	{
		friend class Application;
	private:
		std::string _name = "Un-named";
		Bool _activated = false;

		Widget* _parent = nullptr;
		Float _depth = 0;
		std::vector<Widget*> _childrens;

		std::recursive_mutex _mutex;
		Bool _calculated = false;
		Vector2Int _anchor = Vector2Int(0, 0);
		Vector2Int _size = Vector2Int(0, 0);
		Vector2Int _viewportAnchor = Vector2Int(0, 0);
		Vector2Int _viewportAnchorOffset = Vector2Int(0, 0);
		Bool _cropped = false;
		Vector2Int _viewportSize = Vector2Int(0, 0);
		Vector2Int _viewportSizeOffset = Vector2Int(0, 0);

		void _resetCalculation();
		void _composeViewportInfo();
		Vector2Int _cumulatedAnchor() const;
		void _addChildren(Widget* p_widget);
		void _removeChildren(Widget* p_widget);

		virtual Bool _onUpdate() = 0;
		virtual void _onRender() = 0;
		virtual void _onGeometryChange() = 0;

	protected:
		void _setViewportAnchorOffset(Vector2Int p_anchorOffset);
		void _setViewportSizeOffset(Vector2Int p_sizeOffset);

	public:
		Widget(std::string p_name, Widget* p_parent);
		Widget(Widget* p_parent);

		void setParent(Widget* p_parent);
		std::string name(){return (_name); }
		void setName(std::string p_name);

		void activate();
		void deactivate();
		void setActivate(Bool p_state);
		Bool isActivated();

		void setDepth(Float p_depth);
		void setGeometry(Vector2Int p_anchor, Vector2Int p_size);
		void move(Vector2Int p_deltaAnchor);
		void place(Vector2Int p_anchor);
		Vector2Int anchor() { return (_anchor); }
		Vector2Int size() { return (_size); }
		Float depth() { return (_depth); }
		Vector2Int usableAnchor() { return (_anchor + _viewportAnchorOffset); }
		Vector2Int usableSize() { return (_size - _viewportSizeOffset); }
		Vector2Int viewportAnchor() { return (_viewportAnchor); }
		Vector2Int viewportSize() { return (_viewportSize); }

		Bool update();
		void render();
	};
}