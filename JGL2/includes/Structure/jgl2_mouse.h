#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "jgl2_enums.h"
#include "Structure/jgl2_vector.h"

namespace jgl
{
	class Mouse
	{
	public:

		enum class Button
		{
			Right = 0,
			Left = 1,
			Center = 2
		};

		friend class Application;

	private:
		static const jgl::Size_t C_NB_BUTTON = 3;
		
		jgl::InputStatus _buttons[C_NB_BUTTON];
		jgl::Vector2Int _pos;
		jgl::Float _wheel;

		Mouse();
		void _updateState();

	public:
		jgl::Float wheel() const;
		jgl::Vector2Int pos() const;
		jgl::InputStatus button(Button p_button) const;
	};
}