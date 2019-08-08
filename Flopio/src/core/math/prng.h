#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	uint32_t xorshift128();
	uint32_t xorshift128Limit(int upperLimit);

	void seedUsingRandXorshift128(unsigned int seed1);
	void seedXorshift128(int x, int y, int z, int w);
	double xorshift128d();
	float xorshift128f();

#ifdef __cplusplus
}
#endif
