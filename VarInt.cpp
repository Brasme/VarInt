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

#include "VarInt.h"

static uint8_t char2hex(const char ch) {
	if (ch < '0')
		return 0xff;
	if (ch <= '9')
		return (uint8_t)(ch - '0');
	if (ch < 'A')
		return 0xff;
	if (ch <= 'F')
		return (uint8_t)(ch - 'A') + 10;
	if (ch < 'a')
		return 0xff;
	if (ch <= 'f')
		return (uint8_t)(ch - 'a') + 10;
	return 0xff;
}

VarUint::VarUint() { }

VarUint::VarUint(const char* hexStr) {

	if (hexStr == nullptr)
		return;
	size_t startIdx = 0;
	if (hexStr[startIdx] == '0') {
		if (hexStr[startIdx + 1] == 'x' || hexStr[startIdx + 1] == 'X')
			startIdx = 2;
	}
	size_t endIdx = startIdx;
	while (char2hex(hexStr[endIdx]) != 0xff)
		endIdx++;

	while (endIdx > startIdx) {
		endIdx--;
		uint8_t v = char2hex(hexStr[endIdx]);
		if (endIdx > startIdx) {
			endIdx--;
			v |= char2hex(hexStr[endIdx]) << 4;
		}
		bytes_.push_back(v);
	}
}

VarUint::VarUint(const uint64_t& v) { uint64_t x = v; while (x) { bytes_.push_back((uint8_t)(x & 0xff)); x = x >> 8; } }

VarUint::VarUint(const VarUint& o) { bytes_ = o.bytes_; }

VarUint::VarUint(VarUint&& o) noexcept { std::swap(bytes_, o.bytes_); }

VarUint& VarUint::operator=(const uint64_t& v) { uint64_t x = v; while (x) { bytes_.push_back((uint8_t)(x & 0xff)); x = x >> 8; } return *this; }

VarUint& VarUint::operator=(const VarUint& o) { bytes_ = o.bytes_; return *this; }

VarUint& VarUint::Set(const VarUint& o) { bytes_ = o.bytes_; return *this; }

VarUint& VarUint::operator*=(const VarUint& o) { return Mul(o); }
VarUint& VarUint::operator+=(const VarUint& o) { return Add(o); }
VarUint& VarUint::operator-=(const VarUint& o) { return Sub(o); }
VarUint& VarUint::operator/=(const VarUint& o) { return Div(o); }

bool VarUint::operator==(const VarUint& o) const
{
	auto it1 = bytes_.begin();
	auto it2 = o.bytes_.begin();
	auto itEnd1 = bytes_.end();
	auto itEnd2 = o.bytes_.end();
	while (it1 != itEnd1 && it2 != itEnd2) {
		if (*it1++ != *it2++)
			return false;
	}
	return (it1 == itEnd1 && it2 == itEnd2);
}

bool VarUint::GreaterThan(const VarUint& o,bool orEqual) const
{
	const uint8_t* v1 = bytes_.data();
	const uint8_t* v2 = o.bytes_.data();
	size_t n1 = bytes_.size();
	size_t n2 = o.bytes_.size();
	while (n1 > n2) {
		n1--;
		if (v1[n1])
			return true;
	}
	while (n1 > 0) {
		n1--;
		n2--;
		const uint8_t val1 = v1[n1];
		const uint8_t val2 = v2[n2];
		if (val1 > val2)
			return true;
		if (val1 != val2)
			return false;
	}
	return orEqual;
}

bool VarUint::IsNull() const
{
	if (bytes_.empty())
		return true;
	for (auto it = bytes_.begin(); it != bytes_.end(); ++it)
		if (*it != 0)
			return false;
	return true;
}

VarUint& VarUint::Mul(const VarUint& v) {
	const size_t n0 = bytes_.size();
	const size_t n1 = v.bytes_.size();
	const uint8_t* d0 = bytes_.data();
	const uint8_t* d1 = v.bytes_.data();
	if (n0 + n1 == 0) {
		bytes_.clear();
		return *this;
	}
	std::vector<uint8_t> result(n0 + n1, 0);
	size_t i = 0;
	size_t nNew = 0;
	uint16_t carry = 0;
	for (size_t i1 = 0; i1 < n1; ++i1) {
		const uint16_t b1 = (uint16_t)(d1[i1]);

		// Multiply and add
		carry = 0;
		for (size_t i0 = 0; i0 < n0; ++i0) {
			const uint16_t b0 = (uint16_t)(d0[i0]);
			uint8_t& r = result[i + i0];
			uint16_t v = b0 * b1 + (uint16_t)r + carry;
			r = (uint8_t)(v & 0xff);
			carry = v >> 8;
		}
		result[i + n0] = (uint8_t)carry;
		++i;
	}
	if (carry == 0)
		result.resize(n0 + n1 - 1);
	std::swap(bytes_, result);
	return *this;
}

VarUint& VarUint::Div(const VarUint& v,VarUint *remainReturn)
{
	VarUint divisor(v);
	VarUint remain(*this);
	int i = 0;
	size_t n_r = divisor.Truncate().NumBytes();
	size_t n = Truncate().NumBytes();
	if (n > n_r) {
		i = (int)(n - n_r);
		divisor.LShift(i * 8);
	}
	bytes_.resize((size_t)i + 1, 0);
	while (remain >= divisor && i >= 0) {
		uint8_t digit = remain.MSB() / divisor.MSB();
		VarUint factor;
		factor.Set(divisor).Scale(digit);
		while (digit > 0 && factor > remain) {
			digit--;
			factor.Sub(divisor);
		}
		bytes_[i--] = digit;
		remain.Sub(factor);
		divisor.RShift(8);
	}
	while (i > 0)
		bytes_[i--] = 0;	
	Truncate();
	if (remainReturn)
		*remainReturn = remain;
	return *this;
}

#ifdef OLD
VarUint& VarUint::Mul(const VarUint& v) {
	const size_t n0 = bytes_.size();
	const size_t n1 = v.bytes_.size();
	const uint8_t* d0 = bytes_.data();
	const uint8_t* d1 = v.bytes_.data();
	if (n0 + n1 == 0) {
		bytes_.clear();
		return *this;
	}
	std::vector<uint8_t> result(n0 + n1, 0);
	std::vector<uint8_t> tmp(n0 + 1, 0);
	size_t i = 0;
	size_t nNew = 0;
	uint16_t carry = 0;
	for (size_t i1 = 0; i1 < n1; ++i1) {
		const uint16_t b1 = (uint16_t)(d1[i1]);

		// Multiply
		carry = 0;
		for (size_t i0 = 0; i0 < n0; ++i0) {
			const uint16_t b0 = (uint16_t)(d0[i0]);
			uint16_t v = b0 * b1 + carry;
			tmp[i0] = (uint8_t)(v & 0xff);

			carry = v >> 8;
		}
		tmp[n0] = (uint8_t)carry;

		//Add
		carry = 0;
		for (size_t i0 = 0; i0 < n0; ++i0) {
			uint8_t& r = result[i + i0];
			uint16_t v = (uint16_t)r + (uint16_t)tmp[i0] + carry;
			r = (uint8_t)(v & 0xff);
			carry = v >> 8;
		}
		carry += (uint16_t)tmp[n0];
		result[i + n0] = (uint8_t)carry;
		++i;
	}
	if (carry == 0)
		result.resize(n0 + n1 - 1);
	std::swap(bytes_, result);
	return *this;
}
#endif

VarUint& VarUint::Add(const VarUint& v)
{
	const size_t n0 = bytes_.size();
	const size_t n1 = v.bytes_.size();
	const uint8_t* d1 = v.bytes_.data();
	const size_t n = n0 > n1 ? n0 : n1;
	if (n == 0) {
		bytes_.clear();
		return *this;
	}
	bytes_.resize(n + 1,0);
	uint16_t carry = 0;
	
	size_t i = 0;
	while (i < n1) {
		const uint16_t b1 = (uint16_t)(d1[i]);
		uint8_t& b0 = bytes_[i];
		carry= (uint16_t)b0 + b1 + carry;
		b0=(uint8_t)(carry & 0xff);
		carry = carry >> 8;
		++i;
	}
	while (i < n) {
		uint8_t& b0 = bytes_[i];
		carry = (uint16_t)b0 + carry;
		b0 = (uint8_t)(carry & 0xff);
		carry = carry >> 8;
		++i;
	}
	if (carry) {
		bytes_[i] = (uint8_t)carry;
		return *this;
	}
	bytes_.resize(n);
	return *this;
}

VarUint& VarUint::Sub(const VarUint& v)
{
	size_t n = bytes_.size();
	if (v.bytes_.size() > n)
		n = v.bytes_.size();
	
	Add(VarUint(v).SetNumBytesMinimum(n).Not(false).PlusOne());	
	bytes_.resize(n); // Cuts the additional 1 byte added as result of "2's compliment" add

	size_t nTrunc = 0;
	for (auto it = bytes_.begin(); it != bytes_.end(); ++it)
		nTrunc = (*it == 0) ? nTrunc + 1 : 0;
	if (nTrunc)
		bytes_.resize(bytes_.size()-nTrunc);
	return *this;
}

VarUint& VarUint::Scale(const uint8_t v)
{
	const size_t n = bytes_.size();
	if (n==0) {
		return *this;
	}
	if (v == 0) {
		bytes_.clear();
		return *this;
	}
	const uint16_t scale = (uint16_t)v;
	uint16_t carry = 0;
	for (size_t i = 0; i < n; ++i) {
		uint8_t& b = bytes_[i];
		carry = (uint16_t)b * scale + carry;
		b = (uint8_t)(carry & 0xff);
		carry = carry >> 8;
	}
	if (carry)
		bytes_.push_back((uint8_t)carry);
	return *this;
}

VarUint& VarUint::LShift(const int n)
{
	if (n < 0)
		return RShift(-n);
	
	if (n == 0)
		return *this;

	static uint8_t hexRemain[16] = { 4,3,2,2,1,1,1,1,0,0,0,0,0,0,0,0 };
	size_t bytes = bytes_.size();
	if (bytes == 0)
		return *this; // still == 0
	const uint8_t v = bytes_.back();
	size_t remainBits = hexRemain[v >> 4];
	if (remainBits == 4)
		remainBits += hexRemain[v & 0xf];
	size_t addBytes = (n >> 3);
	size_t addBits = n & 0x7;
	
	size_t numBytes = bytes + addBytes;
	if (addBits > remainBits) {
		++numBytes;
	}
	bytes_.resize(numBytes, 0);
	
	if (addBytes > 0) {
		size_t i = bytes;
		while (i > 0) {
			--i;
			bytes_[addBytes + i] = bytes_[i];
		}
		for (i = 0; i < addBytes; ++i)
			bytes_[i] = 0;
	}

	if (addBits == 0) {
		return *this;
	}

	uint16_t carry = 0;
	size_t idx=addBytes;
	while (idx < numBytes) {
		uint8_t& b = bytes_[idx];
		carry = (((uint16_t)b) << addBits) | carry;
		b = (uint8_t)(carry & 0xff);
		carry = carry >> 8;
		++idx;
	}
	return *this;	
}

VarUint& VarUint::RShift(const int n)
{
	if (n < 0)
		return LShift(-n);

	if (n == 0)
		return *this;

	static uint8_t hexRemain[16] = { 4,3,2,2,1,1,1,1,0,0,0,0,0,0,0,0 };
	size_t bytes = bytes_.size();
	if (bytes == 0)
		return *this; // still == 0
	const uint8_t v = bytes_.back();
	size_t remainBits = hexRemain[v >> 4];
	if (remainBits == 4)
		remainBits += hexRemain[v & 0xf];
	size_t shiftBytes = (n >> 3);
	size_t shiftBits = n & 0x7;
	
	size_t newBytes = shiftBytes >= bytes?0:bytes-shiftBytes;
	if (newBytes==0) {
		bytes_.clear();
		return *this;
	}

	if (shiftBytes > 0) {
		for (size_t i = shiftBytes; i < bytes; ++i)
			bytes_[i - shiftBytes] = bytes_[i];		
	}

	if (shiftBits == 0) {
		bytes_.resize(newBytes);
		return *this;
	}

	uint16_t carry = 0;
	size_t idx = newBytes;
	while (idx > 0) {
		idx--;
		uint8_t& b = bytes_[idx];
		carry = ((uint16_t)b<<(8-shiftBits)) | carry;
		b = (uint8_t)(carry>>8);
		carry = (carry&0xff) << 8;		
	}
	if ((shiftBits + remainBits) >= 8)
		newBytes--;
	bytes_.resize(newBytes);
	return *this;
}

VarUint& VarUint::PlusOne()
{
	for (auto it = bytes_.begin(); it != bytes_.end(); ++it) {
		uint8_t& b = *it;
		const uint8_t v = b;
		if (v < 0xff) {
			b = v + 1;
			return *this;
		}
		b = 0;
	}
	bytes_.push_back(1);
	return *this;
}

VarUint& VarUint::Not(bool truncate)
{
	if (!truncate) {
		for (auto it = bytes_.begin(); it != bytes_.end(); ++it) {
			*it = ~*it;			
		}
		return *this;
	}
	
	size_t nTrunc = 0;
	for (auto it = bytes_.begin(); it != bytes_.end(); ++it) {
		uint8_t& b = *it;
		const uint8_t v = ~b;
		if (v) nTrunc = 0;
		else ++nTrunc;
		b = v;
	}
	if (nTrunc)
		bytes_.resize(bytes_.size() - nTrunc);
	return *this;
}

VarUint& VarUint::And(const VarUint& v, bool truncate)
{
	auto it = bytes_.begin();
	auto it_v = v.bytes_.begin();

	if (!truncate) {
		while (it != bytes_.end() && it_v != v.bytes_.end()) {
			*(it++) &= *(it_v++);
		}
		while (it != bytes_.end()) { // no need to and the remaining... it will and as 0
			*it++ = 0;
		}		
		return *this;
	}

	size_t nTrunc = 0;
	while (it != bytes_.end() && it_v != v.bytes_.end()) {
		const uint8_t v = *it & *(it_v++);
		if (v) nTrunc = 0;
		else nTrunc++;
		*(it++) = v;
	}
	while (it != bytes_.end()) {
		nTrunc++;
		*(it++)=0;
	}
	if (nTrunc)
		bytes_.resize(bytes_.size() - nTrunc);
	return *this;
}

VarUint& VarUint::Or(const VarUint& v)
{
	auto it = bytes_.begin();
	auto it_v = v.bytes_.begin();

	while (it != bytes_.end() && it_v != v.bytes_.end()) {
		*(it++) |= *(it_v++);
	}
	while (it_v != v.bytes_.end()) {
		bytes_.push_back(*(it_v++)); // or non existing ("0") with the new equals new
	}
	// or'ing will never be "shorter".. no need to considder truncate
	return *this;
	
}

VarUint& VarUint::Xor(const VarUint& v, bool truncate)
{
	auto it = bytes_.begin();
	auto it_v = v.bytes_.begin();
	
	if (!truncate) {
		while (it != bytes_.end() && it_v != v.bytes_.end()) {
			*(it++) ^= *(it_v++);			
		}
		while (it_v != v.bytes_.end()) {
			bytes_.push_back(*(it_v++)); // xor non existing ("0") with the new equals new
		}
		return *this;
	}

	size_t nTrunc = 0;	
	while (it != bytes_.end() && it_v != v.bytes_.end()) {
		const uint8_t v = *it ^ *(it_v++);
		if (v) nTrunc = 0;
		else nTrunc++;
		*(it++) = v;
	}
	while (it_v != v.bytes_.end()) {
		const uint8_t v = *(it_v++); // xor non existing ("0") with the new equals new
		if (v) nTrunc = 0;
		else nTrunc++;		
		bytes_.push_back(v); 
	}	
	if (nTrunc)
		bytes_.resize(bytes_.size() - nTrunc);
	return *this;
}

VarUint& VarUint::Truncate()
{
	size_t nTrunc = 0;
	for (auto it = bytes_.begin(); it != bytes_.end(); ++it) {
		if (*it) nTrunc = 0;
		else ++nTrunc;		
	}
	if (nTrunc)
		bytes_.resize(bytes_.size() - nTrunc);
	return *this;
}

uint8_t VarUint::MSB() const { return bytes_.empty() ? 0 : bytes_.back(); }

VarUint::operator uint8_t() const {
	return bytes_.empty() ? 0 : bytes_.front();
}

VarUint::operator uint16_t() const {
	uint16_t v = 0;
	size_t i = 0;
	for (auto it = bytes_.begin(); i < 16 && it != bytes_.end(); ++it) {
		v |= ((uint16_t)(*it)) << i;
		i += 8;
	}
	return v;
}

VarUint::operator uint32_t() const {
	uint32_t v = 0;
	size_t i = 0;
	for (auto it = bytes_.begin(); i < 32 && it != bytes_.end(); ++it) {
		v |= ((uint32_t)(*it)) << i;
		i += 8;
	}
	return v;
}

VarUint::operator uint64_t() const {
	uint64_t v = 0;
	size_t i = 0;
	for (auto it = bytes_.begin(); i < 64 && it != bytes_.end(); ++it) {
		v |= ((uint64_t)(*it)) << i;
		i += 8;
	}
	return v;
}

VarUint& VarUint::SetNumBytesMinimum(size_t n,bool isSigned)
{
	if (n <= bytes_.size())
		return *this;	
	uint8_t padValue = 0;
	if (isSigned) {
		for (auto it = bytes_.begin(); it != bytes_.end(); ++it) {
			uint8_t& b = *it++;
			padValue = (b & 0x80) ? 0xff : 0;
		}			
	}
	bytes_.resize(n, padValue);
	return *this;	
}

std::string VarUint::ToStr(const size_t bytes) const {
	static const char* hexChar = "0123456789ABCDEF";
	size_t m = bytes_.size() * 2;
	if (m != 0) {
		if ((bytes_.back() & 0xf0) == 0)
			m--;
	}
	if (m < 1)
		m = 1;
	size_t n = (bytes > m ? bytes : m) + 2;
	std::string s(n, '0');
	s[1] = 'x';
	for (auto it = bytes_.begin(); it != bytes_.end(); ++it) {
		const uint8_t v = *it;
		n -= 2;
		const uint8_t upper = v >> 4;
		if (upper)
			s[n] = hexChar[upper];
		s[n + 1] = hexChar[v & 0xf];
	}
	return s;
}


std::ostream& operator<<(std::ostream& os, const VarUint& v) { 
	std::ios_base::fmtflags f(os.flags());
	os << v.ToStr();
	os.flags(f);	
	return os; 
}