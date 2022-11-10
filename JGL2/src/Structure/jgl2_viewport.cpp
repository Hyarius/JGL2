#include "Structure/jgl2_viewport.h"
#include "Structure/jgl2_application.h"

namespace jgl
{
	Viewport::Viewport()
	{

	}

	void Viewport::setParentViewport(Viewport* p_parent)
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
		if (_parent == nullptr)
		{
			_anchor = p_anchor;
			_size = p_size;
		}
		else
		{
			_anchor = jgl::clamp(_parent->anchor() + (_parent->_cropped == true ? Vector2Int(0, 0) : _parent->anchorOffset()), p_anchor, _parent->anchor() + _parent->size() - _parent->sizeOffset());
			_size = Vector2Int::composeMin(p_size + p_anchor, _parent->anchor() + (_parent->_cropped == true ? Vector2Int(0, 0) : _parent->anchorOffset()) + _parent->size() - (_parent->_cropped == true ? Vector2Int(0, 0) : _parent->sizeOffset())) - _anchor;

			if (_anchor != p_anchor || _size != p_size)
				_cropped = true;
			else
				_cropped = false;
		}
		_origin = _anchor - p_anchor;
	}

	void Viewport::use()
	{
		C_ORIGIN = _origin;
		jgl::Application::instance()->_setViewport(_anchor, _size);
	}
}