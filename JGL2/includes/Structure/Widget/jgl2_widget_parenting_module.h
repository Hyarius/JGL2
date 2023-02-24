#pragma once

#include "jgl2_includes.h"

namespace jgl
{
	namespace Abstract
	{
		namespace Widget
		{
			class Core;
		}
	}
	namespace Widget
	{
		namespace Module
		{
			class Parenting
			{
			private:
				const Abstract::Widget::Core* _parent = nullptr;
				std::vector<Abstract::Widget::Core*> _childrens;

			public:
				~Parenting();

				const Abstract::Widget::Core* parent() const { return (_parent); }

				void setParent(jgl::Abstract::Widget::Core* p_self, jgl::Abstract::Widget::Core* p_parent);
				void sortChildren();

				void editChildrensGeometry();

				void callChildrenRender();
				bool callChildrenUpdate();

				void addChildren(jgl::Abstract::Widget::Core* p_children);
				const std::vector<Abstract::Widget::Core*>& childrens() { return (_childrens); }
			};
		}
	}
}