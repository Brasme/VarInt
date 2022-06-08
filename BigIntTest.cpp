#include "BigInt.h"

#include <iostream>

void BigIntTest()
{
	// Test0: default construct
	uint64_t v64;
	uint128_t v128;
	uint256_t v256;
	uint512_t v512;
	uint1024_t v1024;
	uint2048_t v2048;
	uint4096_t v4096;

	v2048 = (uint2048_t)v4096;
	v1024 = (uint1024_t)v2048;
}