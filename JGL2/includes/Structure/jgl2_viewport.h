#pragma once

#include "Structure/jgl2_vector2.h"

namespace jgl
{
	namespace Abstract
	{
		namespace Widget
		{
			class Core;
		}
	}

	class Viewport
	{
	private:
		Abstract::Widget::Core* _owner;
		const Viewport* _parent;

		Vector2Int _origin = Vector2Int(0, 0);
		Vector2Int _anchor = Vector2Int(0, 0);
		Vector2Int _cumulatedAnchor = Vector2Int(0, 0);
		Vector2Int _size = Vector2Int(0, 0);
		Vector2Int _cumulatedSize = Vector2Int(0, 0);
		Vector2Int _anchorOffset = Vector2Int(0, 0);
		Vector2Int _sizeOffset = Vector2Int(0, 0);

	public:
		Viewport(Abstract::Widget::Core* p_owner);

		const Vector2Int& anchor() const { return (_anchor); }
		const Vector2Int& size() const { return (_size); }
		const Vector2Int& anchorOffset() const { return (_anchorOffset); }
		const Vector2Int& sizeOffset() const { return (_sizeOffset); }

		void setOffset(Vector2Int p_anchorOffset, Vector2Int p_sizeOffset);
		void setParentViewport(const Viewport* p_parent);
		void configure(Vector2Int p_anchor, jgl::Vector2Int p_size);
		void use() const;
	};
}