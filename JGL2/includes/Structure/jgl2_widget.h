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
		std::string _widgetName = "Un-named";
		jgl::Bool _activated = false;

		Widget* _parent = nullptr;
		std::vector<Widget*> _childrens;

		std::recursive_mutex _mutex;
		Bool _calculated = false;
		jgl::Vector2Int _anchor = jgl::Vector2Int(0, 0);
		jgl::Vector2Int _size = jgl::Vector2Int(0, 0);
		jgl::Vector2Int _viewportAnchor = jgl::Vector2Int(0, 0);
		jgl::Vector2Int _viewportAnchorOffset = jgl::Vector2Int(0, 0);
		jgl::Vector2Int _viewportSize = jgl::Vector2Int(0, 0);
		jgl::Vector2Int _viewportSizeOffset = jgl::Vector2Int(0, 0);

		void _resetCalculation();
		void _composeViewportInfo();
		Vector2Int _cumulatedAnchor() const;
		void _addChildren(Widget* p_widget);
		void _removeChildren(Widget* p_widget);

		virtual jgl::Bool _onUpdate() = 0;
		virtual void _onRender() = 0;
		virtual void _onGeometryChange() = 0;

	protected:
		void _setViewportAnchorOffset(jgl::Vector2Int p_anchorOffset);
		void _setViewportSizeOffset(jgl::Vector2Int p_sizeOffset);

	public:
		Widget(std::string p_widgetName, Widget* p_parent);
		Widget(Widget* p_parent);

		void setParent(Widget* p_parent);
		std::string name(){return (_widgetName); }
		void setName(std::string p_name);

		void activate();
		void deactivate();
		void setActivate(jgl::Bool p_state);
		jgl::Bool isActivated();

		void setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size);
		void move(jgl::Vector2Int p_deltaAnchor);
		void place(jgl::Vector2Int p_anchor);
		Vector2Int anchor() { return (_anchor); }
		Vector2Int size() { return (_size); }
		Vector2Int usableAnchor() { return (_anchor + _viewportAnchorOffset); }
		Vector2Int usableSize() { return (_size - _viewportSizeOffset); }
		Vector2Int viewportAnchor() { return (_viewportAnchor); }
		Vector2Int viewportSize() { return (_viewportSize); }

		jgl::Bool update();
		void render();
	};
}