#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "Structure/jgl2_vector.h"
#include "Structure/jgl2_viewport.h"

namespace jgl
{
	class Application;

	class Widget
	{
		friend class Application;
	private:
		std::string _name = "Un-named";
		Bool _activated = false;

		Viewport _viewport;
		Widget* _parent = nullptr;
		Float _depth = 0;
		std::vector<Widget*> _childrens;

		std::recursive_mutex _mutex;
		Bool _calculated = false;
		Vector2Int _anchor = Vector2Int(0, 0);
		Vector2Int _size = Vector2Int(0, 0);

		void _resetCalculation();
		void _composeViewportInfo();
		Vector2Int _cumulatedAnchor() const;
		void _addChildren(Widget* p_widget);
		void _removeChildren(Widget* p_widget);

		virtual Bool _onUpdate() = 0;
		virtual void _onRender() = 0;
		virtual void _onGeometryChange() = 0;

	protected:
		void _setViewportOffset(Vector2Int p_anchorOffset, Vector2Int p_sizeOffset);

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

		Bool isPointed() const;

		void setDepth(Float p_depth);
		void setGeometry(Vector2Int p_anchor, Vector2Int p_size);
		void move(Vector2Int p_deltaAnchor);
		void place(Vector2Int p_anchor);
		Viewport* viewport() { return (&_viewport); }
		Vector2Int anchor() { return (_anchor); }
		Vector2Int size() { return (_size); }
		Vector2Int anchorOffset() { return (_viewport.anchorOffset()); }
		Vector2Int sizeOffset() { return (_viewport.sizeOffset()); }
		Float depth() { return (_depth); }
		Vector2Int usableAnchor() { return (_anchor + anchorOffset()); }
		Vector2Int usableSize() { return (_size - sizeOffset()); }

		Bool update();
		void render();
	};
}