#include "Structure/Widget/jgl2_widget_parenting_manager.h"
#include "Structure/Widget/jgl2_widget_core.h"

namespace jgl
{
	WidgetParentingManager::~WidgetParentingManager()
	{
		for (WidgetCore* child : _childrens)
			delete child;
	}

	void WidgetParentingManager::setParent(jgl::WidgetCore* p_self, jgl::WidgetCore* p_parent)
	{
		_parent = p_parent;

		if (_parent != nullptr)
		{
			p_parent->_parentingManager.addChildren(p_self);
			p_self->_viewport.setParentViewport(p_parent->viewport());
		}
	}
	
	void WidgetParentingManager::sortChildren()
	{
		std::sort(_childrens.begin(), _childrens.end(), [](jgl::WidgetCore* lhs, jgl::WidgetCore* rhs) {
				return lhs->depth() > rhs->depth();
			});
	}

	void WidgetParentingManager::editChildrensGeometry()
	{
		for (WidgetCore* child : _childrens)
		{
			child->_geometryEdited = true;
		}
	}

	void WidgetParentingManager::callChildrenRender()
	{
		for (WidgetCore* child : _childrens)
		{
			if (child->_isActive == true)
			{
				child->_render();
			}
		}
	}

	bool WidgetParentingManager::callChildrenUpdate()
	{
		for (WidgetCore* child : _childrens)
		{
			if (child->_isActive == true)
			{
				if (child->_update() == true)
					return (true);
			}
		}
		return (false);
	}

	void WidgetParentingManager::addChildren(jgl::WidgetCore* p_children)
	{
		_childrens.push_back(p_children);
	}
}