#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t seed[] = { 5946 , 751614 , 801223 , 1120847 };

void seedUsingRandXorshift128(unsigned int seed1) 
{
	srand(seed1);
	seed[0] = rand();
	seed[1] = rand();
	seed[2] = rand();
	seed[3] = rand();
}

void seedXorshift128(int x, int y, int z, int w) 
{
	seed[0] = x;
	seed[1] = y;
	seed[2] = z;
	seed[3] = w;
}

uint32_t xorshift128() 
{
	uint32_t s, t = seed[3];
	t ^= t << 11;
	t ^= t >> 8;
	seed[3] = seed[2]; seed[2] = seed[1]; seed[1] = s = seed[0];
	t ^= s;
	t ^= s >> 19;
	seed[0] = t;
	return t;
}

uint32_t xorshift128Limit(int upperLimit)
{
	uint32_t s, t = seed[3];
	t ^= t << 11;
	t ^= t >> 8;
	seed[3] = seed[2]; seed[2] = seed[1]; seed[1] = s = seed[0];
	t ^= s;
	t ^= s >> 19;
	seed[0] = t;
	return t % upperLimit;
}

double xorshift128d() 
{
	return xorshift128() / 0xFFFFFFFF;
}

float xorshift128f() 
{
	return (float)xorshift128() / 0xFFFFFFFF;
}
