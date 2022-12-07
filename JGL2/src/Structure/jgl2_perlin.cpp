#include "Structure/jgl2_perlin.h"
#include "Structure/jgl2_vector.h"
#include "jgl2_basic_functions.h"

namespace jgl
{
	jgl::Float Perlin::_computeWaveLength(jgl::Float p_x, jgl::Float p_y, jgl::Float p_frequency)
	{
		p_x /= p_frequency;
		p_y /= p_frequency;

		jgl::Vector2 directions[8] = {
			jgl::Vector2(0.707f, 0.707f),
			jgl::Vector2(-0.707f, 0.707f),
			jgl::Vector2(0.707f, -0.707f),
			jgl::Vector2(-0.707f, -0.707f),

			jgl::Vector2(1.0f, 0.0f),
			jgl::Vector2(-1.0f, 0.0f),
			jgl::Vector2(0.0f, 1.0f),
			jgl::Vector2(0.0f, -1.0f),
		};

		jgl::Vector2 targetPoint = jgl::Vector2(p_x, p_y);
		jgl::Vector2 pointA = jgl::Vector2((int)(p_x), (int)(p_y));
		jgl::Vector2 pointB = jgl::Vector2((int)(p_x + 1), (int)(p_y));
		jgl::Vector2 pointC = jgl::Vector2((int)(p_x), (int)(p_y + 1));
		jgl::Vector2 pointD = jgl::Vector2((int)(p_x + 1), (int)(p_y + 1));
		jgl::Vector2 targetDelta = targetPoint - pointA;

		jgl::Int ii = static_cast<jgl::Int>(pointA.x()) & 255;
		jgl::Int jj = static_cast<jgl::Int>(pointA.y()) & 255;

		jgl::Int directionIndexA = jgl::generateNumberFromSeed(_seed, (ii + jgl::generateNumberFromSeed(_seed, jj))) % 8;
		jgl::Int directionIndexB = jgl::generateNumberFromSeed(_seed, (ii + 1 + jgl::generateNumberFromSeed(_seed, jj))) % 8;
		jgl::Int directionIndexC = jgl::generateNumberFromSeed(_seed, (ii + jgl::generateNumberFromSeed(_seed, jj + 1))) % 8;
		jgl::Int directionIndexD = jgl::generateNumberFromSeed(_seed, (ii + 1 + jgl::generateNumberFromSeed(_seed, jj + 1))) % 8;

		jgl::Float dotA = directions[directionIndexA].dot(targetPoint - pointA);
		jgl::Float dotB = directions[directionIndexB].dot(targetPoint - pointB);
		jgl::Float dotC = directions[directionIndexC].dot(targetPoint - pointC);
		jgl::Float dotD = directions[directionIndexD].dot(targetPoint - pointD);

		jgl::Float smoothX = 3 * targetDelta.x() * targetDelta.x() - 2 * targetDelta.x() * targetDelta.x() * targetDelta.x();
		jgl::Float smoothY = 3 * targetDelta.y() * targetDelta.y() - 2 * targetDelta.y() * targetDelta.y() * targetDelta.y();

		jgl::Float dotInterpolate1 = dotA + smoothX * (dotB - dotA);
		jgl::Float dotInterpolate2 = dotC + smoothX * (dotD - dotC);

		jgl::Float percent = dotInterpolate1 + smoothY * (dotInterpolate2 - dotInterpolate1);

		return (percent);
	}

	Perlin::Perlin(jgl::ULong p_seed) :
		_seed(p_seed),
		_min(),
		_max(),
		_range(),
		_octaveValue(3),
		_frequency(50),
		_persistance(0.5f),
		_lacunarity(2.0f)
	{

	}

	void Perlin::configureSeed(jgl::ULong p_seed)
	{
		_seed = p_seed;
	}

	void Perlin::configureFrequency(jgl::Float p_frequency)
	{
		_frequency = p_frequency;
	}

	void Perlin::configurePersistance(jgl::Float p_persistance)
	{
		_persistance = p_persistance;
	}

	void Perlin::configureLacunarity(jgl::Float p_lacunarity)
	{
		_lacunarity = p_lacunarity;
	}

	void Perlin::configureOctave(jgl::Size_t p_octaveValue)
	{
		_octaveValue = p_octaveValue;
	}

	void Perlin::configureRange(jgl::Float p_min, jgl::Float p_max)
	{
		_min = p_min;
		_max = p_max;
		_range = _max - _min;
	}

	jgl::Float Perlin::sample2D(jgl::Float p_x, jgl::Float p_y)
	{
		p_x = std::fabs(p_x + _seed);
		p_y = std::fabs(p_y + _seed);

		jgl::Float result = 0;
		jgl::Float amplitude = 1.0f;
		jgl::Float frequency = _frequency;

		for (jgl::Size_t i = 0; i < _octaveValue; i++)
		{
			result += _computeWaveLength(p_x, p_y, frequency) * amplitude;
			amplitude *= _persistance;
			frequency /= _lacunarity;
		}

		result += 1;
		result /= 2;
		result *= _range;
		result += _min;

		return (result);
	}

	const jgl::ULong& Perlin::seed() const
	{
		return (_seed);
	}
}