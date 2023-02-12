#include "Structure/Widget/jgl2_widget_core.h"
#include "jgl2_basic_functions.h"
#include "Structure/jgl2_iostream.h"

namespace jgl
{
	void WidgetCore::_render()
	{
		if (_geometryEdited == true)
		{
			_onGeometryChange();
			_geometryEdited = false;
		}
		if (_parentingManager.parent() != nullptr)
			_parentingManager.parent()->viewport()->use();
		_onRender();
		_parentingManager.callChildrenRender();
	}

	bool WidgetCore::_update()
	{
		if (_parentingManager.callChildrenUpdate() == true)
			return (true);
		return _onUpdate();
	}

	WidgetCore::WidgetCore(std::string p_name) :
		_name(p_name),
		_viewport(this)
	{

	}
	WidgetCore::~WidgetCore()
	{

	}

	void WidgetCore::setDepth(jgl::Float p_depth)
	{
		jgl::Float delta = p_depth - _depth;
		_depth = p_depth;
		_geometryEdited = true;
		for (jgl::Size_t i = 0; i < _parentingManager.childrens().size(); i++)
		{
			_parentingManager.childrens()[i]->setDepth(_parentingManager.childrens()[i]->depth() + delta);
		}
		_parentingManager.sortChildren();
	}

	void WidgetCore::activate()
	{
		setActivationStatus(true);
	}
	
	void WidgetCore::deactivate()
	{
		setActivationStatus(false);
	}
	
	void WidgetCore::setActivationStatus(jgl::Bool p_state)
	{
		_isActive = p_state;
	}

	void WidgetCore::setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size)
	{
		_anchor = p_anchor;
		_size = p_size;
		_viewport.configure(p_anchor, p_size);
		_geometryEdited = true;
		_parentingManager.editChildrensGeometry();
	}
}