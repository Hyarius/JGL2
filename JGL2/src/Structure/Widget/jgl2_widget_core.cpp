#include "Structure/Widget/jgl2_widget_core.h"
#include "jgl2_basic_functions.h"
#include "Structure/jgl2_iostream.h"
#include "Structure/Application/graphical/jgl2_application.h"

namespace jgl::Abstract::Widget
{
	void Core::_render()
	{
		if (_geometryEdited == true)
		{
			_onGeometryChange();
			_geometryEdited = false;
		}
		if (_parentingModule.parent() != nullptr)
			_parentingModule.parent()->viewport()->use();
		_onRender();
		_parentingModule.callChildrenRender();
	}

	bool Core::_update()
	{
		if (_parentingModule.callChildrenUpdate() == true)
			return (true);
		return _onUpdate();
	}

	Core::Core(std::string p_name) :
		_name(p_name),
		_viewport(this)
	{

	}
	Core::~Core()
	{

	}

	void Core::setDepth(jgl::Float p_depth)
	{
		jgl::Float delta = p_depth - _depth;
		_depth = p_depth;
		_geometryEdited = true;
		for (jgl::Size_t i = 0; i < _parentingModule.childrens().size(); i++)
		{
			_parentingModule.childrens()[i]->setDepth(_parentingModule.childrens()[i]->depth() + delta);
		}
		_parentingModule.sortChildren();
	}

	void Core::activate()
	{
		setActivationStatus(true);
	}
	
	void Core::deactivate()
	{
		setActivationStatus(false);
	}
	
	void Core::setActivationStatus(jgl::Bool p_state)
	{
		_isActive = p_state;
	}

	void Core::setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size)
	{
		_anchor = p_anchor;
		_size = p_size;
		_viewport.configure(p_anchor, p_size);
		_parentingModule.editChildrensGeometry();
		_geometryEdited = true;
	}

	jgl::Bool Core::isPointed()
	{
		Vector2Int pos = jgl::Application::Graphical::instance()->mouse()->pos();

		return (pos.isBetween(_viewport.anchor(), _viewport.anchor() + _viewport.size()));
	}
}