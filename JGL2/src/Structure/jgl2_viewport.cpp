#include "Structure/jgl2_viewport.h"
#include "Structure/Application/Graphical/jgl2_application.h"
#include "Structure/Widget/jgl2_widget_core.h"

namespace jgl
{
	Viewport::Viewport(jgl::Abstract::Widget::Core* p_owner)
	{
		_owner = p_owner;
	}

	void Viewport::setParentViewport(const Viewport* p_parent)
	{
		_parent = p_parent;
	}

	void Viewport::setOffset(Vector2Int p_anchorOffset, Vector2Int p_sizeOffset)
	{
		_anchorOffset = p_anchorOffset;
		_sizeOffset = p_sizeOffset;
	}

	void Viewport::configure(Vector2Int p_anchor, jgl::Vector2Int p_size)
	{
		_anchor = p_anchor;
		_size = p_size;

		if (_parent == nullptr)
		{
			_origin = _anchor + _anchorOffset;
			_cumulatedAnchor = _anchor + _anchorOffset;
			_cumulatedSize = _size - _sizeOffset;
		}
		else
		{
			_origin = _parent->_origin + _anchor + _anchorOffset;

			_cumulatedAnchor = jgl::Vector2Int::max(_parent->_cumulatedAnchor, _parent->_cumulatedAnchor + _anchor + _anchorOffset);
			_cumulatedSize = jgl::Vector2Int::min(_parent->_cumulatedAnchor + _parent->_size, _origin + _size - _sizeOffset) - _cumulatedAnchor;
		}
	}

	void Viewport::use() const
	{
		jgl::Application::Graphical::instance()->_window.setOrigin(_origin);
		jgl::Application::Graphical::instance()->_window.setScissorViewport(_cumulatedAnchor, _cumulatedSize);
	}
}