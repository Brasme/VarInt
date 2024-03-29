﻿
/* Bent Gramdal, 2022, bent@gramdal.no
 * VarUint - any number of byte representation of integer. Not made for performance but a example on "how to" 
 * implement math on any size integers, https://github.com/Brasme/VarInt
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following condition:
 * 
 * The above copyright notice and this permission notice should be (but not required) included or referenced in portions
 *  of the Software where used.
 *   
 */ 


#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include <ostream>

class VarUint {
public:
	VarUint();
	VarUint(const char* hexStr);
	VarUint(const uint64_t& v);
	VarUint(const VarUint& o);
	VarUint(VarUint&& o) noexcept;

	VarUint& operator=(const uint64_t& v);
	VarUint& operator=(const VarUint& o);

	VarUint& Set(const VarUint& o);

	VarUint& operator *=(const VarUint& o);
	VarUint& operator +=(const VarUint& o);
	VarUint& operator -=(const VarUint& o);
	VarUint& operator /=(const VarUint& o);
	
	bool operator ==(const VarUint& o) const;
	inline bool operator !=(const VarUint& o) const { return !operator==(o); }
	inline bool operator >(const VarUint& o) const { return GreaterThan(o); }
	inline bool operator >=(const VarUint& o) const { return GreaterThan(o,true); }
	inline bool operator <(const VarUint& o) const { return !GreaterThan(o,true); }
	inline bool operator <=(const VarUint& o) const { return !GreaterThan(o); }
	bool GreaterThan(const VarUint& o,bool orEqual = false) const;
	bool IsNull() const;
	static const VarUint& Null();

	VarUint& Mul(const VarUint& v);
	VarUint& Div(const VarUint& v, VarUint* remain=nullptr);
	VarUint& Add(const VarUint& v);
	VarUint& Sub(const VarUint& v);
	VarUint& Scale(const uint8_t v);
	VarUint& LShift(const int n);
	VarUint& RShift(const int n);

	VarUint& PlusOne();
	VarUint& Not(bool truncate=true);
	VarUint& And(const VarUint& v, bool truncate = true);
	VarUint& Or(const VarUint& v);
	VarUint& Xor(const VarUint& v, bool truncate = true);

	VarUint& Truncate();
	uint8_t MSB() const;

	operator uint8_t() const;
	operator uint16_t() const;
	operator uint32_t() const;
	operator uint64_t() const;
	
	uint64_t UintFromBytes(size_t fromDigit,size_t toDigit=0) const;

	inline VarUint& SetNumBytesMinimum(const VarUint& v) { return SetNumBytesMinimum(v.bytes_.size(),false); }
	VarUint& SetNumBytesMinimum(size_t n,bool isSigned=false);
	inline size_t NumBytes() const { return bytes_.size(); }

	std::string ToHexStr(const size_t bytes = 0) const;
	std::string ToDecStr() const;
private:
	std::vector<uint8_t> bytes_;
};

inline VarUint operator *(const VarUint& v1, const VarUint& v2) { return VarUint(v1).Mul(v2); }
inline VarUint operator +(const VarUint& v1, const VarUint& v2) { return VarUint(v1).Add(v2); }
inline VarUint operator -(const VarUint& v1, const VarUint& v2) { return VarUint(v1).Sub(v2); }
inline VarUint operator /(const VarUint& v1, const VarUint& v2) { return VarUint(v1).Div(v2); }
inline VarUint operator !(const VarUint& v1) { return VarUint(v1).Not(); }
inline VarUint operator ~(const VarUint& v1) { return VarUint(v1).Not(); }
inline VarUint operator ^(const VarUint& v1, const VarUint& v2) { return VarUint(v1).Xor(v2); }
inline VarUint operator &(const VarUint& v1, const VarUint& v2) { return VarUint(v1).And(v2); }
inline VarUint operator |(const VarUint& v1, const VarUint& v2) { return VarUint(v1).Or(v2); }

std::ostream& operator<<(std::ostream& os, const VarUint& v);