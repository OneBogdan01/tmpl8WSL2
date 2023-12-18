#pragma once
#include "template.h"
// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

// In this file: implementation of various functions of the template
// math library defined in tmpl8math.h.


// random number generator - Marsaglia's xor32
// This is a high-quality RNG that uses a single 32-bit seed. More info:
// https://www.researchgate.net/publication/5142825_Xorshift_RNGs

// RNG seed. NOTE: in a multithreaded application, don't use a single seed!
class RandomNumberGenerator
{
public:
	inline static uint seed = 0x12345678;

	// WangHash: calculates a high-quality seed based on an arbitrary non-zero
	// integer. Use this to create your own seed based on e.g. thread index.
	static uint WangHash(uint s)
	{
		s = (s ^ 61) ^ (s >> 16);
		s *= 9, s = s ^ (s >> 4);
		s *= 0x27d4eb2d;
		s = s ^ (s >> 15);
		return s;
	}

	static uint InitSeed(uint seedBase)
	{
		return WangHash((seedBase + 1) * 17);
	}

	// RandomUInt()
	// Update the seed and return it as a random 32-bit unsigned int.
	static uint RandomUInt()
	{
		seed ^= seed << 13;
		seed ^= seed >> 17;
		seed ^= seed << 5;
		return seed;
	}

	// RandomFloat()
	// Calculate a random unsigned int and cast it to a float in the range
	// [0..1)
	float RandomFloat()
	{
		return RandomUInt() * 2.3283064365387e-10f;
	}

	float Rand(float range)
	{
		return RandomFloat() * range;
	}

	// Calculate a random number based on a specific seed
	uint RandomUInt(uint& customSeed)
	{
		customSeed ^= customSeed << 13;
		customSeed ^= customSeed >> 17;
		customSeed ^= customSeed << 5;
		return customSeed;
	}

	float RandomFloat(uint& customSeed)
	{
		return RandomUInt(customSeed) * 2.3283064365387e-10f;
	}

	// Perlin noise implementation - https://stackoverflow.com/questions/29711668/perlin-noise-generation
	inline static int numX = 512, numY = 512, numOctaves = 7, primeIndex = 0;
	inline static float persistence = 0.5f;
	inline static int primes[10][3] = {
		{995615039, 600173719, 701464987}, {831731269, 162318869, 136250887}, {174329291, 946737083, 245679977},
		{362489573, 795918041, 350777237}, {457025711, 880830799, 909678923}, {787070341, 177340217, 593320781},
		{405493717, 291031019, 391950901}, {458904767, 676625681, 424452397}, {531736441, 939683957, 810651871},
		{997169939, 842027887, 423882827}
	};

	static float Noise(const int i, const int x, const int y)
	{
		int n = x + y * 57;
		n = (n << 13) ^ n;
		const int a = primes[i][0], b = primes[i][1], c = primes[i][2];
		const int t = (n * (n * n * a + b) + c) & 0x7fffffff;
		return 1.0f - (float)t / 1073741824.0f;
	}

	static float SmoothedNoise(const int i, const int x, const int y)
	{
		const float corners = (Noise(i, x - 1, y - 1) + Noise(i, x + 1, y - 1) + Noise(i, x - 1, y + 1) +
			Noise(i, x + 1, y + 1)) / 16;
		const float sides = (Noise(i, x - 1, y) + Noise(i, x + 1, y) + Noise(i, x, y - 1) + Noise(i, x, y + 1)) / 8;
		const float center = Noise(i, x, y) / 4;
		return corners + sides + center;
	}

	static float Interpolate(const float a, const float b, const float x)
	{
		const float ft = x * 3.1415927f, f = (1 - cosf(ft)) * 0.5f;
		return a * (1 - f) + b * f;
	}

	static float InterpolatedNoise(const int i, const float x, const float y)
	{
		const int integer_X = (int)x, integer_Y = (int)y;
		const float fractional_X = x - integer_X, fractional_Y = y - integer_Y;
		const float v1 = SmoothedNoise(i, integer_X, integer_Y);
		const float v2 = SmoothedNoise(i, integer_X + 1, integer_Y);
		const float v3 = SmoothedNoise(i, integer_X, integer_Y + 1);
		const float v4 = SmoothedNoise(i, integer_X + 1, integer_Y + 1);
		const float i1 = Interpolate(v1, v2, fractional_X);
		const float i2 = Interpolate(v3, v4, fractional_X);
		return Interpolate(i1, i2, fractional_Y);
	}

	float noise2D(const float x, const float y)
	{
		float total = 0, frequency = (float)(2 << numOctaves), amplitude = 1;
		for (int i = 0; i < numOctaves; ++i)
		{
			frequency /= 2, amplitude *= persistence;
			total += InterpolatedNoise((primeIndex + i) % 10, x / frequency, y / frequency) * amplitude;
		}
		return total / frequency;
	}
};
