#pragma once

#include "Structure/jgl2_vector.h"

namespace jgl
{
	class Viewport
	{
	public:
		static inline Vector2Int C_ORIGIN = Vector2Int(0, 0);
		static inline Vector2Int C_SCREEN_ANCHOR = Vector2Int(0, 0);
		static inline Vector2Int C_SCREEN_SIZE = Vector2Int(0, 0);
		
	private:

		Viewport* _parent;
		Bool _cropped = false;
		Vector2Int _origin = Vector2Int(0, 0);
		Vector2Int _anchor = Vector2Int(0, 0);
		Vector2Int _size = Vector2Int(0, 0);
		Vector2Int _anchorOffset = Vector2Int(0, 0);
		Vector2Int _sizeOffset = Vector2Int(0, 0);

	public:
		static void reset();

		Viewport();

		const Vector2Int& origin() const { return (_origin); }
		const Vector2Int& anchor() const { return (_anchor); }
		const Vector2Int& size() const { return (_size); }
		const Vector2Int& anchorOffset() const { return (_anchorOffset); }
		const Vector2Int& sizeOffset() const { return (_sizeOffset); }

		void setOffset(Vector2Int p_anchorOffset, Vector2Int p_sizeOffset);
		void setParentViewport(Viewport* p_parent);
		void configure(Vector2Int p_anchor, jgl::Vector2Int p_size);
		void use();
	};
}