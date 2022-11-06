#include "Structure/jgl2_widget.h"
#include "Structure/jgl2_iostream.h"
#include "Structure/jgl2_application.h"

namespace jgl
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

	void Widget::setName(std::string p_name)
	{
		_widgetName = p_name;
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

	void Widget::_setViewportAnchorOffset(jgl::Vector2Int p_anchorOffset)
	{
		_viewportAnchorOffset = p_anchorOffset;
	}
	
	void Widget::_setViewportSizeOffset(jgl::Vector2Int p_sizeOffset)
	{
		_viewportSizeOffset = p_sizeOffset;
	}

	void Widget::_composeViewportInfo()
	{
		if (_parent == nullptr)
		{
			_viewportAnchor = _viewportAnchorOffset;
			_viewportSize = jgl::Application::instance()->size() - _viewportSizeOffset;
		}
		else
		{
			_viewportAnchor = _parent->_cumulatedAnchor() + _viewportAnchorOffset;
			_viewportSize = _parent->size() - _viewportSizeOffset;

			if (_viewportAnchor.x() < _parent->viewportAnchor().x() + _viewportAnchorOffset.x())
				_viewportAnchor.x() = _parent->viewportAnchor().x() + _viewportAnchorOffset.x();
			if (_viewportAnchor.y() < _parent->viewportAnchor().y() + _viewportAnchorOffset.y())
				_viewportAnchor.y() = _parent->viewportAnchor().y() + _viewportAnchorOffset.y();

			if (_viewportAnchor.x() + _viewportSize.x() > _parent->viewportAnchor().x() + _parent->viewportSize().x() - _viewportSizeOffset.x())
				_viewportSize.x() = _parent->viewportAnchor().x() + _parent->viewportSize().x() - _viewportAnchor.x() - _viewportSizeOffset.x();
			if (_viewportAnchor.y() + _viewportSize.y() > _parent->viewportAnchor().y() + _parent->viewportSize().y() - _viewportSizeOffset.y())
				_viewportSize.y() = _parent->viewportAnchor().y() + _parent->viewportSize().y() - _viewportAnchor.y() - _viewportSizeOffset.y();
		}
	}

	void Widget::setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size)
	{
		_anchor = p_anchor;
		_size = p_size;
		_calculated = false;
		_onGeometryChange();
	}

	void Widget::move(jgl::Vector2Int p_deltaAnchor)
	{
		setGeometry(_anchor + p_deltaAnchor, _size);
	}

	void Widget::place(jgl::Vector2Int p_anchor)
	{
		setGeometry(p_anchor, _size);
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

	Vector2Int Widget::_cumulatedAnchor() const
	{
		Vector2Int result;
		const Widget* tmp = this;
		while (tmp != nullptr)
		{
			result += tmp->_anchor + tmp->_viewportAnchorOffset;
			tmp = tmp->_parent;
		}
		return (result);
	}

	void Widget::render()
	{
		if (_activated == false)
			return;

		if (_calculated == false)
		{
			_composeViewportInfo();
			_calculated = true;
		}

		jgl::Application::instance()->_setViewport(_viewportAnchor, _viewportSize);
		_onRender();
		for (jgl::Size_t i = 0; i < _childrens.size(); i++)
		{
			_childrens[i]->render();
		}
	}
}