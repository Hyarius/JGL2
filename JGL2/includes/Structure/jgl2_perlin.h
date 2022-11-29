#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

namespace jgl
{
	class Perlin
	{
	private:
		jgl::ULong _seed;
		jgl::Float _min;
		jgl::Float _max;
		jgl::Float _range;
		jgl::Size_t _octaveValue;
		jgl::Float _frequency;
		jgl::Float _persistance;
		jgl::Float _lacunarity;

		jgl::Float _computeWaveLength(jgl::Float p_x, jgl::Float p_y, jgl::Float p_frequency);

	public:
		Perlin(jgl::ULong p_seed = 12500);
		void configureFrequency(jgl::Float p_frequency);
		void configurePersistance(jgl::Float p_persistance);
		void configureLacunarity(jgl::Float p_lacunarity);
		void setOctaveValue(jgl::Size_t p_octaveValue);
		void configureRange(jgl::Float p_min, jgl::Float p_max);
		jgl::Float sample2D(jgl::Float p_x, jgl::Float p_y);
	};
}