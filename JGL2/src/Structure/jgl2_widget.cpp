#include "Structure/jgl2_widget.h"
#include "Structure/jgl2_iostream.h"
#include "Structure/jgl2_application.h"

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
				_parent->_removeChildren(this);
			else
			{
				auto tmp = std::find(jgl::Application::instance()->_widgets.begin(), jgl::Application::instance()->_widgets.end(), this);

				if (tmp != jgl::Application::instance()->_widgets.end())
				{
					jgl::Application::instance()->_widgets.erase(tmp);
				}
			}

			_parent = p_parent;

			if (_parent != nullptr)
				_parent->_addChildren(this);
			else
				jgl::Application::instance()->_widgets.push_back(this);
		}

		jgl::Bool Widget::update()
		{
			return (_onUpdate());
		}

		void Widget::render()
		{
			_onRender();
		}

	}
}