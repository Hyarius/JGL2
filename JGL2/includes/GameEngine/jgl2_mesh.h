#pragma once

#include "jgl2.h"

struct Mesh
{
	jgl::Image* texture;
	std::vector<jgl::Vector3> vertices;
	std::vector<jgl::Vector2> uvs;
	std::vector<jgl::UInt> elements;
};