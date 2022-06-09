// Plans: Could use gtest. If anyone find it useful its fine - added permission in VarInt.h/.cpp

#include "VarInt.h"
#include "gtest/gtest.h"

TEST(VarInt,Division) 
{
	VarUint v = VarUint("0x123456789abcdef123456789abcdef123456") / VarUint("0x123");
	EXPECT_EQ(v.ToStr(), "0x1003D0E8C777876D3505A257AEE4AB9BE4");

	v = VarUint("0x1234") / VarUint("0x12345");
	EXPECT_EQ(v.ToStr(), "0x0");

	v = VarUint("0x12345") / VarUint("0x12345");
	EXPECT_EQ(v.ToStr(), "0x1");

	v = VarUint("0x123456") / VarUint("0x12345");
	EXPECT_EQ(v.ToStr(), "0x10");

	v = VarUint("0x1234567") / VarUint("0x12345");
	EXPECT_EQ(v.ToStr(), "0x100");

	v = VarUint("0x12345678") / VarUint("0x12345");
	EXPECT_EQ(v.ToStr(), "0x1000");

	v = VarUint("0x123456789") / VarUint("0x12345");
	EXPECT_EQ(v.ToStr(), "0x10000");

	v = VarUint("0x123456789a") / VarUint("0x12345");
	EXPECT_EQ(v.ToStr(), "0x100005");

	v = VarUint("0x123456789ab") / VarUint("0x12345");
	EXPECT_EQ(v.ToStr(), "0x100005B");

	v = VarUint("0x123456789abc") / VarUint("0x12345");
	EXPECT_EQ(v.ToStr(), "0x100005B0");

	v = VarUint("0x123456789abcd") / VarUint("0x12345");
	EXPECT_EQ(v.ToStr(), "0x100005B00");

	v = VarUint("0x123456789abcd") / VarUint("0x123");
	EXPECT_EQ(v.ToStr(), "0x1003D0E8C77");
}


int main(int argc, char** argv) {
	printf("Running main() from %s\n", __FILE__);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}