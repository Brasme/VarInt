// Plans: Could use gtest. If anyone find it useful its fine - added permission in VarInt.h/.cpp

#include "VarInt.h"

#include <iostream>
#include <iomanip>

template<typename T=uint64_t,unsigned width=0>
struct Hex {
	Hex(const T& value): value(value) {}
	const T value;
};

template<typename T,unsigned width=0>
std::ostream& operator<<(std::ostream& os, const Hex<T,width>& asHex) { 
	std::ios_base::fmtflags f(os.flags());
	os << "0x" << std::hex << std::setfill('0');
	if (width)
		os << std::setw(width);
	os << asHex.value;
	os.flags(f);
	return os;
}

void Test_Hex()
{
	std::cout << "Test: value as hex string\n";
	std::cout << "0x1122*0x22334   =    0x249f20e8 (Hex<>, ok int*int)       =" << Hex<>(0x1122 * 0x22334) << std::endl; // Warning: Signed integral overflow (int*int)
	std::cout << "0x112233*0x223344= 0x249f9233e8c (Hex<>, overflow int*int) =" << Hex<>(0x112233 * 0x223344) << std::endl; // Warning: Signed integral overflow (int*int)
	std::cout << "0x112233*0x223344= 0x249f9233e8c (Hex<>, ok)               =" << Hex<>((uint64_t)0x112233 * 0x223344) << std::endl;
	std::cout << "0x112233*0x223344= 0x249f9233e8c (Hex<uint64_t>, ok as 16 chars)   =" << Hex<uint64_t,16>((uint64_t)0x112233 * 0x223344) << std::endl;

	// Output:
	// Test: value as hex string
	// 0x112233 * 0x223344 (Hex<uint32_t>, overflow uint32_t) = 0xf9233e8c
	// 0x112233 * 0x223344 (Hex<uint64_t>, ok) = 0x249f9233e8c
	// 0x112233 * 0x223344 (Hex<uint64_t>, ok as 16 chars) = 0x00000249f9233e8c
}


void VarIntTest()
{
	Test_Hex();

	std::cout << "\nTest1:\n";
	{
		VarUint v1;
		VarUint v2(0x123);
		VarUint v3 = 0x12345;

		std::cout << "v1=" << v1 << std::endl;
		std::cout << "v2=" << v2 << std::endl;
		std::cout << "v3=" << v3 << std::endl;

		VarUint v4 = v2 * v3;
		std::cout << "v4=v2*v3=" << v4 << "(==0x14B176F)" << std::endl;
	}

	std::cout << "\nTest2:\n";
	{
		VarUint v1(0xff);
		VarUint v2 = 0xff;

		std::cout << "v1=" << v1 << std::endl;
		std::cout << "v2=" << v2 << std::endl;

		VarUint v3 = v1 * v2;
		std::cout << "v3=v1*v2=" << v3 << "(==0xFE01)" << std::endl;
	}

	std::cout << "\nTest3:\n";
	{
		VarUint v1(0xffffffff);
		VarUint v2 = 0xffffffff;

		std::cout << "v1=" << v1 << std::endl;
		std::cout << "v2=" << v2 << std::endl;
		
		std::cout << "v1*v2=" << Hex<uint64_t>((uint64_t)v1 * (uint64_t)v2) << std::endl;
		
		VarUint v3 = v1 * v2;
		std::cout << "v3=v1*v2=" << v3 << "(==0xFFFFFFFE00000001)" << std::endl;
	}

	std::cout << "\nTest4:\n";
	{
		const char* v1Str = "0xfedcba9876543210123456789ABCDEF123456";
		const char* v2Str = "0x123ABCDEFfedcba9876543210123456789ABCDEF123456FFFF";

		VarUint v1(v1Str);
		VarUint v2 = v2Str;

		std::cout << "v1=" << v1 << "(==" << v1Str << ")" << std::endl;
		std::cout << "v2=" << v2 << "(==" << v2Str << ")" << std::endl;
	}

	std::cout << "\nTest5-mul:\n";
	{
		VarUint v1("ffffffffffffffffffffffffffff");
		VarUint v2 = 0x123456789abcdef0ull;

		std::cout << "v1=" << v1 << "(==0xffffffffffffffffffffffffffff)" << std::endl;
		std::cout << "v2=" << v2 << "(==0x123456789abcdef0)" << std::endl;

		VarUint v3 = v1 * v2;
		std::cout << "v3=v1*v2=" << v3 << "(==0x123456789ABCDEEFFFFFFFFFFFFFEDCBA98765432110)" << std::endl;
	}

	std::cout << "\nTest5-add:\n";
	{
		VarUint v1("ffffffffffffffffffffffffffff");
		VarUint v2 = 0x123456789abcdef0ull;

		std::cout << "v1=" << v1 << "(==0xffffffffffffffffffffffffffff)" << std::endl;
		std::cout << "v2=" << v2 << "(==0x123456789abcdef0)" << std::endl;

		VarUint v3 = v1 + v2;
		std::cout << "v3=v1+v2=" << v3 << "(==0x1000000000000123456789ABCDEEF)" << std::endl;
	}

	std::cout << "\nTest5-sub:\n";
	{
		VarUint v1("ffffffffffffffffffffffffffff");
		VarUint v2 = 0x123456789abcdef0ull;

		std::cout << "v1=" << v1 << "(==0xffffffffffffffffffffffffffff)" << std::endl;
		std::cout << "v2=" << v2 << "(==0x123456789abcdef0)" << std::endl;

		VarUint v3 = v1 - v2;
		std::cout << "v3=v1-v2=" << v3 << "(==0xFFFFFFFFFFFFEDCBA9876543210F)" << std::endl;

		std::cout << "=> 0x123-0x111=" << (VarUint("0x123")-VarUint("0x111")) << std::endl;
		std::cout << "=> 0x123-0x121=" << (VarUint("0x123") - VarUint("0x121")) << std::endl;
		std::cout << "=> 0x123-0x125=" << (VarUint("0x123") - VarUint("0x125")) << std::endl;
	}

	std::cout << "\nTest6:\n";
	{
		uint64_t vu1 = 0xffeeddccbbaa9988;
		uint64_t vu2 = 0xfedcba98789abcde;

		VarUint v1(vu1);
		VarUint v2 = vu2;

		std::cout << "v1=" << v1 << "(==0xffeeddccbbaa9988)" << std::endl;
		std::cout << "v2=" << v2 << "(==0xfedcba98789abcde)" << std::endl;

		VarUint v3 = v1 * v2;
		std::cout << "v3=v1*v2=" << v3 << "(==0xFECBABE3B9B1A75502DF86B4228103F0)" << std::endl;
	}

	std::cout << "\nTest7-scale:\n";
	{
		VarUint v1("ba98765456789abcdefaaaaaaaaaf");

		std::cout << "v1=" << v1 << "(==ba98765456789abcdefaaaaaaaaaf)" << std::endl;

		VarUint v2 = v1.Scale(0xff);
		std::cout << "v1.Scale(0xff)" << v1 << "(==0xb9d dddd e022 2222 2221 bb00 0000 0451)" << std::endl;
	}

	std::cout << "\nTest7-l-shift:\n";
	{
		{
			const size_t n = 1;
			VarUint v("0x1");
			std::cout << "v=" << v; v.LShift(n); std::cout << " << " << n << " = " << v << std::endl;
		}

		{
			const size_t n = 1;
			VarUint v("0x7");
			std::cout << "v=" << v; v.LShift(n); std::cout << " << " << n << " = " << v << std::endl;
		}

		{
			const size_t n = 1;
			VarUint v("0xf");
			std::cout << "v=" << v; v.LShift(n); std::cout << " << " << n << " = " << v << std::endl;
		}

		{
			const size_t n = 1;
			VarUint v("0x1248");
			std::cout << "v=" << v; v.LShift(n); std::cout << " << " << n << " = " << v << std::endl;
		}

		{
			const size_t n = 9;
			VarUint v("0xabcdef12345678");
			std::cout << "v=" << v;
			v.LShift(n);
			std::cout << " << " << n << " = " << v << "(==0x1579bde2468acf000)" << std::endl;
		}

		{
			const size_t n = 13;
			VarUint v("0xfacb7f463d297453fedc");
			std::cout << "v=" << v;
			v.LShift(n);
			std::cout << " << " << n << " = " << v << "(==0x1f596fe8c7a52e8a7fdb8000)" << std::endl;
		}

	}

	std::cout << "\nTest7-l-shift:\n";
	{
		{
			const size_t n = 1;
			VarUint v("0x1");
			std::cout << "v=" << v; v.LShift(n); std::cout << " << " << n << " = " << v << "(==0x2)" << std::endl;
		}

		{
			const size_t n = 1;
			VarUint v("0x7");
			std::cout << "v=" << v; v.LShift(n); std::cout << " << " << n << " = " << v << "(==0xe)" << std::endl;
		}

		{
			const size_t n = 1;
			VarUint v("0xf");
			std::cout << "v=" << v; v.LShift(n); std::cout << " << " << n << " = " << v << "(== 0x1e)" << std::endl;
		}

		{
			const size_t n = 1;
			VarUint v("0x1248");
			std::cout << "v=" << v; v.LShift(n); std::cout << " << " << n << " = " << v << "(==0x2490)" << std::endl;
		}

		{
			const size_t n = 9;
			VarUint v("0xabcdef12345678");
			std::cout << "v=" << v;
			v.LShift(n);
			std::cout << " << " << n << " = " << v << "(==0x1579bde2468acf000)" << std::endl;
		}

		{
			const size_t n = 13;
			VarUint v("0xfacb7f463d297453fedc");
			std::cout << "v=" << v;
			v.LShift(n);
			std::cout << " << " << n << " = " << v << "(==0x1f596fe8c7a52e8a7fdb8000)" << std::endl;
		}
	}

	std::cout << "\nTest7-r-shift:\n";
	{
		{
			const size_t n = 1;
			VarUint v("0x3");
			std::cout << "v=" << v; v.RShift(n); std::cout << " >> " << n << " = " << v << "(==0x1)" << std::endl;
		}

		{
			const size_t n = 1;
			VarUint v("0xf");
			std::cout << "v=" << v; v.RShift(n); std::cout << " >> " << n << " = " << v << "(==0x7)" << std::endl;
		}

		{
			const size_t n = 1;
			VarUint v("0x1f");
			std::cout << "v=" << v; v.RShift(n); std::cout << " >> " << n << " = " << v << "(== 0xf)" << std::endl;
		}

		{
			const size_t n = 1;
			VarUint v("0x2491");
			std::cout << "v=" << v; v.RShift(n); std::cout << " >> " << n << " = " << v << "(==0x1248)" << std::endl;
		}

		{
			const size_t n = 9;
			VarUint v("0x1579bde2468acf1aa");
			std::cout << "v=" << v;
			v.RShift(n);
			std::cout << " >> " << n << " = " << v << "(==0xabcdef12345678)" << std::endl;
		}

		{
			const size_t n = 13;
			VarUint v("0x1f596fe8c7a52e8a7fdb9fff");
			std::cout << "v=" << v;
			v.RShift(n);
			std::cout << " >> " << n << " = " << v << "(==0xfacb7f463d297453fedc)" << std::endl;
		}
	}
}
