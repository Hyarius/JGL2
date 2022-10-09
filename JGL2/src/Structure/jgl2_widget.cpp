#include "Structure/jgl2_widget.h"
#include "Structure/jgl2_iostream.h"

namespace jgl
{
	namespace Abstract
	{
		void Widget::_addChildren(Widget* p_parent)
		{

		}

		void Widget::_removeChildren(Widget* p_parent)
		{

		}

		Widget::Widget(jgl::String p_widgetName, Widget* p_parent) :
			_widgetName(p_widgetName)
		{
			setParent(p_parent);
		}

		Widget::Widget(Widget* p_parent) : Widget("Unnamed", p_parent)
		{

		}

		void Widget::setParent(Widget* p_parent)
		{
			if (_parent != nullptr)
			{
				_parent->_removeChildren(this);
			}
			else
			{

			}
			_parent = p_parent;
		}

		jgl::Bool Widget::update()
		{
			jgl::cout << "Updating widget " << _widgetName << jgl::endl;
			return (false);
		}

		void Widget::render()
		{
			jgl::cout << "Rendering widget " << _widgetName << jgl::endl;
		}

	}
}