// BigInt.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <stdint.h>

	
#ifdef __cplusplus


// variadic template with one or more arguments.
// ellipsis (...) operator to the left of the parameter name declares a parameter pack,
// allowing you to declare zero or more parameters (of different types).
template <typename First, typename... Args>
void Foo(First first, Args... args) {
	//std::cout << __PRETTY_FUNCTION__ << "\n";
	Foo(first);
	Foo(args...);
	//  ellipsis (...) operator to the right of the parameter name will cause
	//  the whole expression that precedes the ellipsis to be repeated for every
	//  subsequent argument unpacked from the argument pack, with the expressions
	//  separated by commas.
}

template<class T>
struct lowHighUint_t {
	lowHighUint_t() : low(),high() {}
	lowHighUint_t(const T &l) : low(l), high() {}	
	lowHighUint_t(const lowHighUint_t& o) : low(o.low), high(o.high) {}
	lowHighUint_t& operator=(const T& l) { low = l; high = 0; return *this; }
	lowHighUint_t& operator=(const lowHighUint_t& o) { low = o.low; high = o.high; return *this; }
	operator T() const { return low; }
	T low;
	T high;
};

typedef lowHighUint_t<uint64_t> uint128_t;
typedef lowHighUint_t<uint128_t> uint256_t;
typedef lowHighUint_t<uint256_t> uint512_t;
typedef lowHighUint_t<uint512_t> uint1024_t;
typedef lowHighUint_t<uint1024_t> uint2048_t;
typedef lowHighUint_t<uint2048_t> uint4096_t;

#else // C
struct _uint128_t {
	uint64_t low;
	uint64_t high;
};

struct _uint256_t {
	_uint128_t low;
	_uint128_t high;
	};

struct _uint512_t {
	_uint256_t low;
	_uint256_t high;
};

struct _uint1024_t {
	_uint512_t low;
	_uint512_t high;
};

struct _uint2048_t {
	_uint1024_t low;
	_uint1024_t high;
};

struct _uint4096_t {
	_uint2048_t low;
	_uint2048_t high;
};

typedef struct _uint128_t uint128_t;
typedef struct _uint256_t uint256_t;
typedef struct _uint512_t uint512_t;
typedef struct _uint1024_t uint1024_t;
typedef struct _uint2048_t uint2048_t;
typedef struct _uint4096_t uint4096_t;

#endif
