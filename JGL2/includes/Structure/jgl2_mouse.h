#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "jgl2_enums.h"
#include "Structure/jgl2_vector2.h"

namespace jgl
{
	class Mouse
	{
		friend class MouseManager;

	public:
		enum class Button
		{
			Right = 0,
			Left = 1,
			Center = 2
		};

	private:
		static const jgl::Size_t C_NB_BUTTON = 3;
		
		jgl::InputStatus _buttons[C_NB_BUTTON];
		jgl::Vector2Int _pos;
		jgl::Float _wheel;

		void _updateState();

	public:
		Mouse();
		jgl::Float wheel() const;
		jgl::Vector2Int pos() const;
		jgl::InputStatus getButton(Button p_button) const;
	};

	std::string to_string(const Mouse::Button& p_button);
}