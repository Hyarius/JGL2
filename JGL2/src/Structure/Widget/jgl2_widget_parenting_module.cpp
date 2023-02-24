#include "Structure/Widget/jgl2_widget_parenting_module.h"
#include "Structure/Widget/jgl2_widget_core.h"

namespace jgl::Widget::Module
{
	Parenting::~Parenting()
	{
		for (jgl::Abstract::Widget::Core* child : _childrens)
			delete child;
	}

	void Parenting::setParent(jgl::Abstract::Widget::Core* p_self, jgl::Abstract::Widget::Core* p_parent)
	{
		_parent = p_parent;

		if (_parent != nullptr)
		{
			p_parent->_parentingModule.addChildren(p_self);
			p_self->_viewport.setParentViewport(p_parent->viewport());
		}
	}
	
	void Parenting::sortChildren()
	{
		std::sort(_childrens.begin(), _childrens.end(), [](jgl::Abstract::Widget::Core* lhs, jgl::Abstract::Widget::Core* rhs) {
				return lhs->depth() > rhs->depth();
			});
	}

	void Parenting::editChildrensGeometry()
	{
		for (jgl::Abstract::Widget::Core* child : _childrens)
		{
			child->_geometryEdited = true;
		}
	}

	void Parenting::callChildrenRender()
	{
		for (jgl::Abstract::Widget::Core* child : _childrens)
		{
			if (child->_isActive == true)
			{
				child->_render();
			}
		}
	}

	bool Parenting::callChildrenUpdate()
	{
		for (jgl::Abstract::Widget::Core* child : _childrens)
		{
			if (child->_isActive == true)
			{
				if (child->_update() == true)
					return (true);
			}
		}
		return (false);
	}

	void Parenting::addChildren(jgl::Abstract::Widget::Core* p_children)
	{
		_childrens.push_back(p_children);
	}
}