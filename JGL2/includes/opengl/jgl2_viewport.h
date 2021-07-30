#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

namespace jgl
{
	class Viewport
	{
	private:

		const class Widget* _owner;
		static jgl::Map< jgl::Uint, bool> _allocated_id;
		jgl::Uint _id;
		Vector2Int _anchor;
		Vector2Int _area;

		~Viewport()
		{
			_allocated_id[_id] = false;
		}

	public:
		/*
			Return the anchor of the viewport in pixel
		*/
		const jgl::Vector2Int anchor() const { return (_anchor); }

		/*
			Return the area of the viewport in pixel
		*/
		const jgl::Vector2Int area() const { return (_area); }

		/*
			Base constructor
			
			Optional : 
			jgl::Widget *p_owner - Define who owns this viewport 
		*/
		Viewport(class Widget*p_owner = nullptr);

		/*
			Define the anchor and area of this viewport
		*/
		void set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area);

		/*
			Reset window viewport to basic "all screen" settings
		*/
		void reset();

		/*
			Limit the rendering part of the screen to (anchor) / (anchor + area) part of the screen
			Also set the active viewport of the jgl::Application to this viewport
		*/
		void use();
	};
}