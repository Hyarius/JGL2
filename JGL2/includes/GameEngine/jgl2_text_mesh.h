#pragma once

#include "jgl2.h"

struct TextMesh
{
	jgl::Font* font;
	std::string text = "";
	jgl::Color textColor = jgl::Color(255, 255, 255);
	jgl::Float textWorldHeight = 1;
	jgl::Size_t oulineSize = 2;
	jgl::Color outlineColor = jgl::Color(0, 0, 0);
	jgl::Vector3 textWorldArea;
	jgl::VerticalAlignment verticalAlignment;
	jgl::HorizontalAlignment horizontalAlignment;
};