#include "Structure/Widget/jgl2_widget.h"
#include "Structure/jgl2_iostream.h"
#include "Structure/jgl2_graphical_application.h"

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

	Widget::Widget(std::string p_name, Widget* p_parent) :
		_name(p_name),
		_activated(false),
		_viewport(this)
	{
		setParent(p_parent);

		if (jgl::GraphicalApplication::instance() != nullptr)
		{
			if (p_parent == nullptr)
			{
				setDepth(0);
			}
			else
			{
				setDepth(p_parent->depth() + 1);
			}
		}
	}

	Widget::Widget(Widget* p_parent) : Widget("Unnamed", p_parent)
	{

	}

	void Widget::setName(std::string p_name)
	{
		_name = p_name;
	}

	Widget* Widget::parent() const
	{
		return (_parent);
	}

	void Widget::setParent(Widget* p_parent)
	{
		if (_parent != nullptr)
			_parent->_removeChildren(this);
		else
		{
			auto tmp = std::find(jgl::AbstractApplication::_mainApplication->_widgets.begin(), jgl::AbstractApplication::_mainApplication->_widgets.end(), this);
			
			if (tmp != jgl::AbstractApplication::_mainApplication->_widgets.end())
			{
				jgl::AbstractApplication::_mainApplication->_widgets.erase(tmp);
			}
		}

		_parent = p_parent;

		if (_parent != nullptr)
		{
			_parent->_addChildren(this);
			_viewport.setParentViewport(p_parent->viewport());
		}
		else
		{
			jgl::AbstractApplication::_mainApplication->_widgets.push_back(this);
			_viewport.setParentViewport(nullptr);
		}
	}
	
	void Widget::_onDeactivation()
	{

	}
	
	void Widget::_onActivation()
	{

	}

	void Widget::activate()
	{
		if (_activated == false)
			_onActivation();
		_activated = true;
	}

	void Widget::deactivate()
	{
		if (_activated == true)
			_onDeactivation();
		_activated = false;
	}
		
	void Widget::setActivate(jgl::Bool p_state)
	{
		_activated = p_state;
	}

	Bool Widget::isPointed() const
	{
		Vector2Int pos = jgl::GraphicalApplication::instance()->mouse().pos();

		return (pos.isBetween(_viewport.anchor(), _viewport.anchor() + _viewport.size()));
	}

	jgl::Bool Widget::isActivated()
	{
		return (_activated);
	}

	void Widget::_setViewportOffset(Vector2Int p_anchorOffset, Vector2Int p_sizeOffset)
	{
		_viewport.setOffset(p_anchorOffset, p_sizeOffset);
	}

	void Widget::_composeViewportInfo()
	{
		_viewport.configure(_cumulatedAnchor(), _size);
	}

	void Widget::_resetCalculation()
	{
		_calculated = false;
		for (jgl::Size_t i = 0; i < _childrens.size(); i++)
		{
			_childrens[i]->_resetCalculation();
		}
	}

	void Widget::sortChildrenByDepth()
	{
		std::sort(_childrens.begin(), _childrens.end(), [](jgl::Widget* lhs, jgl::Widget* rhs) {
			return lhs->depth() > rhs->depth();
			});
	}

	void Widget::setDepth(jgl::Float p_depth)
	{
		if (p_depth > jgl::GraphicalApplication::instance()->maxDepth())
			throw std::out_of_range("Widget depth out of range");

		jgl::Float delta = p_depth - _depth;
		_depth = p_depth;
		_resetCalculation();
		_recalcPositionChange();
		for (jgl::Size_t i = 0; i < _childrens.size(); i++)
		{
			_childrens[i]->setDepth(_childrens[i]->depth() + delta);
		}
		sortChildrenByDepth();
	}

	void Widget::setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size)
	{
		_anchor = p_anchor;
		_size = p_size;
		_resetCalculation();
		_onGeometryChange();
	}

	void Widget::move(jgl::Vector2Int p_deltaAnchor)
	{
		_anchor += p_deltaAnchor;
		_resetCalculation();
		_recalcPositionChange();
	}
	 
	void Widget::place(jgl::Vector2Int p_anchor)
	{
		_anchor = p_anchor;
		_resetCalculation();
		_recalcPositionChange();
	}

	void Widget::_recalcPositionChange()
	{
		_composeViewportInfo();
		_onPositionChange();
		for (jgl::Size_t i = 0; i < _childrens.size(); i++)
		{
			_childrens[i]->_recalcPositionChange();
		}
	}

	void Widget::_onPositionChange()
	{
		
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
		Vector2Int result = this->anchor();
		const Widget* tmp = this->_parent;
		while (tmp != nullptr)
		{
			result += tmp->_anchor + tmp->_viewport.anchorOffset();
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

		if (_parent == nullptr)
		{
			Viewport::reset();
		}

		_viewport.use();
		_onRender();
		for (jgl::Size_t i = 0; i < _childrens.size(); i++)
		{
			_childrens[i]->render();
		}
	}
}