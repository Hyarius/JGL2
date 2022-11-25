#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

namespace jgl
{
	class Perlin {
	private:
		jgl::UChar p[512];

		jgl::Float fade(jgl::Float t);
		jgl::Float lerp(jgl::Float t, jgl::Float a, jgl::Float b);
		jgl::Float grad(jgl::UChar hash, jgl::Float x, jgl::Float y, jgl::Float z);
		jgl::Float weight(jgl::Int octaves);

	public:
		Perlin(jgl::Int seed = std::default_random_engine::default_seed);

		void reseed(jgl::UInt seed);
		jgl::Float noise(jgl::Float x, jgl::Float y = 0, jgl::Float z = 0, jgl::Float p_zoom = 1);
		jgl::Float accumulated_noise(jgl::Int octaves, jgl::Float x, jgl::Float y = 0, jgl::Float z = 0, jgl::Float p_zoom = 1);
	};
}