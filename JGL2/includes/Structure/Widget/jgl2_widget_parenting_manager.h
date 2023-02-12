#pragma once

#include "jgl2_includes.h"

namespace jgl
{
	class WidgetCore;

	class WidgetParentingManager
	{
	private:
		const WidgetCore* _parent = nullptr;
		std::vector<WidgetCore*> _childrens;

	public:
		~WidgetParentingManager();

		const WidgetCore* parent() const { return (_parent); }

		void setParent(jgl::WidgetCore* p_self, jgl::WidgetCore* p_parent);
		void sortChildren();

		void editChildrensGeometry();

		void callChildrenRender();
		bool callChildrenUpdate();

		void addChildren(jgl::WidgetCore* p_children);
		const std::vector<WidgetCore*>& childrens() { return (_childrens); }
	};
}