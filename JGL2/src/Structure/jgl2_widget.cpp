#include "Structure/jgl2_widget.h"
#include "Structure/jgl2_iostream.h"
#include "Structure/jgl2_application.h"

namespace jgl
{
	namespace Abstract
	{
		void Widget::_addChildren(Widget* p_widget)
		{
			_childrens.push_back(p_widget);
		}

		void Widget::_removeChildren(Widget* p_widget)
		{
			auto tmp = std::find(_childrens.begin(), _childrens.end(), p_widget);
			if (tmp != _childrens.end())
				_childrens.erase(tmp);
		}

		Widget::Widget(std::string p_widgetName, Widget* p_parent) :
			_widgetName(p_widgetName),
			_activated(false)
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

		void Widget::activate()
		{
			_activated = true;
		}

		void Widget::deactivate()
		{
			_activated = false;
		}
		
		void Widget::setActivate(jgl::Bool p_state)
		{
			_activated = p_state;
		}
		
		jgl::Bool Widget::isActivated()
		{
			return (_activated);
		}

		jgl::Bool Widget::update()
		{
			if (_activated == false)
				return false;

			for (jgl::Size_t i = 0; i < _childrens.size(); i++)
			{
				if (_childrens[i]->update() == true)
					return (true);
			}
			return (_onUpdate());
		}

		void Widget::render()
		{
			if (_activated == false)
				return;

			_onRender();
			for (jgl::Size_t i = 0; i < _childrens.size(); i++)
			{
				_childrens[i]->render();
			}
		}

	}
}