#include "Structure/jgl2_viewport.h"
#include "Structure/jgl2_application.h"
#include "Structure/Widget/jgl2_widget.h"

namespace jgl
{
	Viewport::Viewport(Widget* p_owner)
	{
		_owner = p_owner;
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
		_croppedTop = false;
		_croppedDown = false;
		_croppedLeft = false;
		_croppedRight = false;
		if (_parent == nullptr)
		{
			_anchor = p_anchor;
			_size = p_size;
		}
		else
		{
			jgl::Vector2Int parentAnchorOffsetCropped = Vector2Int(
				(_parent->_croppedLeft == true ? 0 : _parent->anchorOffset().x()),
				(_parent->_croppedTop == true ? 0 : _parent->anchorOffset().y())
			);

			jgl::Vector2Int parentSizeOffsetCropped = Vector2Int(
				(_parent->_croppedRight == true ? _parent->sizeOffset().x() / 2 : _parent->sizeOffset().x()),
				(_parent->_croppedDown == true ? _parent->sizeOffset().y() / 2 : _parent->sizeOffset().y())
			);

			jgl::Vector2Int selfTopLeftCorner = p_anchor;
			jgl::Vector2Int selfDownRightCorner = p_anchor + p_size;

			jgl::Vector2Int parentTopLeftCorner = _parent->anchor() + parentAnchorOffsetCropped;
			jgl::Vector2Int parentDownRightCorner = parentTopLeftCorner + _parent->size() - parentSizeOffsetCropped;

			if (selfTopLeftCorner.x() < parentTopLeftCorner.x())
				_croppedLeft = true;
			if (selfTopLeftCorner.y() < parentTopLeftCorner.y())
				_croppedTop = true;

			if (selfDownRightCorner.x() > parentDownRightCorner.x())
				_croppedRight = true;
			if (selfDownRightCorner.y() > parentDownRightCorner.y())
				_croppedDown = true;

			_anchor = jgl::clamp(_parent->anchor() + parentAnchorOffsetCropped, p_anchor, _parent->anchor() + _parent->size() - _parent->sizeOffset());
			_size = Vector2Int::composeMin(p_size + p_anchor, _parent->anchor() + parentAnchorOffsetCropped + _parent->size() - parentSizeOffsetCropped) - _anchor;
		}
		_origin = _anchor - p_anchor;
	}

	void Viewport::use()
	{
		C_ORIGIN = _origin;
		jgl::Application::instance()->_setViewport(_anchor, _size);
	}
}