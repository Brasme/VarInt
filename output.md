# Code output

	Test: value as hex string
    0x1122*0x22334   =    0x249f20e8 (Hex<>, ok int*int)       =0x249f20e8
    0x112233*0x223344= 0x249f9233e8c (Hex<>, overflow int*int) =0xfffffffff9233e8c
    0x112233*0x223344= 0x249f9233e8c (Hex<>, ok)               =0x249f9233e8c
    0x112233*0x223344= 0x249f9233e8c (Hex<uint64_t>, ok as 16 chars)   =0x00000249f9233e8c
    
    Test1:
    v1=0x0
    v2=0x123
    v3=0x12345
    v4=v2*v3=0x14B176F(==0x14B176F)
    
    Test2:
    v1=0xFF
    v2=0xFF
    v3=v1*v2=0xFE01(==0xFE01)
    
    Test3:
    v1=0xFFFFFFFF
    v2=0xFFFFFFFF
    v1*v2=0xfffffffe00000001
    v3=v1*v2=0xFFFFFFFE00000001(==0xFFFFFFFE00000001)
    
    Test4:
    v1=0xFEDCBA9876543210123456789ABCDEF123456(==0xfedcba9876543210123456789ABCDEF123456)
    v2=0x123ABCDEFFEDCBA9876543210123456789ABCDEF123456FFFF(==0x123ABCDEFfedcba9876543210123456789ABCDEF123456FFFF)
    
    Test5-mul:
    v1=0xFFFFFFFFFFFFFFFFFFFFFFFFFFFF(==0xffffffffffffffffffffffffffff)
    v2=0x123456789ABCDEF0(==0x123456789abcdef0)
    v3=v1*v2=0x123456789ABCDEEFFFFFFFFFFFFFEDCBA98765432110(==0x123456789ABCDEEFFFFFFFFFFFFFEDCBA98765432110)
    
    Test5-add:
    v1=0xFFFFFFFFFFFFFFFFFFFFFFFFFFFF(==0xffffffffffffffffffffffffffff)
    v2=0x123456789ABCDEF0(==0x123456789abcdef0)
    v3=v1+v2=0x1000000000000123456789ABCDEEF(==0x1000000000000123456789ABCDEEF)
    
    Test5-sub:
    v1=0xFFFFFFFFFFFFFFFFFFFFFFFFFFFF(==0xffffffffffffffffffffffffffff)
    v2=0x123456789ABCDEF0(==0x123456789abcdef0)
    v3=v1-v2=0xFFFFFFFFFFFFEDCBA9876543210F(==0xFFFFFFFFFFFFEDCBA9876543210F)
    => 0x123-0x111=0x12
    => 0x123-0x121=0x2
    => 0x123-0x125=0xFFFE
    
    Test6:
    v1=0xFFEEDDCCBBAA9988(==0xffeeddccbbaa9988)
    v2=0xFEDCBA98789ABCDE(==0xfedcba98789abcde)
    v3=v1*v2=0xFECBABE3B9B1A75502DF86B4228103F0(==0xFECBABE3B9B1A75502DF86B4228103F0)
    
    Test7-scale:
    v1=0xBA98765456789ABCDEFAAAAAAAAAF(==ba98765456789abcdefaaaaaaaaaf)
    v1.Scale(0xff)0xB9DDDDDE02222222221BB0000000451(==0xb9d dddd e022 2222 2221 bb00 0000 0451)
    
    Test7-l-shift:
    v=0x1 << 1 = 0x2
    v=0x7 << 1 = 0xE
    v=0xF << 1 = 0x1E
    v=0x1248 << 1 = 0x2490
    v=0xABCDEF12345678 << 9 = 0x1579BDE2468ACF000(==0x1579bde2468acf000)
    v=0xFACB7F463D297453FEDC << 13 = 0x1F596FE8C7A52E8A7FDB8000(==0x1f596fe8c7a52e8a7fdb8000)
    
    Test7-l-shift:
    v=0x1 << 1 = 0x2(==0x2)
    v=0x7 << 1 = 0xE(==0xe)
    v=0xF << 1 = 0x1E(== 0x1e)
    v=0x1248 << 1 = 0x2490(==0x2490)
    v=0xABCDEF12345678 << 9 = 0x1579BDE2468ACF000(==0x1579bde2468acf000)
    v=0xFACB7F463D297453FEDC << 13 = 0x1F596FE8C7A52E8A7FDB8000(==0x1f596fe8c7a52e8a7fdb8000)
    
    Test7-r-shift:
    v=0x3 >> 1 = 0x1(==0x1)
    v=0xF >> 1 = 0x7(==0x7)
    v=0x1F >> 1 = 0xF(== 0xf)
    v=0x2491 >> 1 = 0x1248(==0x1248)
    v=0x1579BDE2468ACF1AA >> 9 = 0xABCDEF12345678(==0xabcdef12345678)
    v=0x1F596FE8C7A52E8A7FDB9FFF >> 13 = 0xFACB7F463D297453FEDC(==0xfacb7f463d297453fedc)
    
    Test8-Div:
             (uint64_t)(VarUint(0x123456789abcdef123456789abcdef123456)) / 0x123  =0x5b15b42bed969c
     VarUint(0x123456789abcdef123456789abcdef123456)/ VarUint(0x123) =0x1003D0E8C777876D3505A257AEE4AB9BE4
             0x1234       /         0x12345  =0x0
     VarUint(0x1234)      / VarUint(0x12345) =0x0
             0x12345      /         0x12345  =0x1
     VarUint(0x12345)     / VarUint(0x12345) =0x1
             0x123456     /         0x12345  =0x10
     VarUint(0x123456)    / VarUint(0x12345) =0x10
             0x1234567    /         0x12345  =0x100
     VarUint(0x1234567)   / VarUint(0x12345) =0x100
             0x12345678   /         0x12345  =0x1000
     VarUint(0x12345678)  / VarUint(0x12345) =0x1000
             0x123456789  /         0x12345  =0x10000
     VarUint(0x123456789) / VarUint(0x12345) =0x10000
             0x123456789a    /         0x12345  =0x100005
     VarUint(0x123456789a)   / VarUint(0x12345) =0x100005
             0x123456789ab   /         0x12345  =0x100005b
     VarUint(0x123456789ab)  / VarUint(0x12345) =0x100005B
             0x123456789abc  /         0x12345  =0x100005b0
     VarUint(0x123456789abc) / VarUint(0x12345) =0x100005B0
             0x123456789abcd /         0x12345  =0x100005b00
     VarUint(0x123456789abcd)/ VarUint(0x12345) =0x100005B00
             0x123456789abcd /         0x123  =0x1003d0e8c77
     VarUint(0x123456789abcd)/ VarUint(0x123) =0x1003D0E8C77
    
    C:\Users\Bent\source\BigInt\out\build\x64-Debug\BigAndVarIntTest.exe (process 4040) exited with code 0.
    To automatically close the console when debugging stops, enable Tools->Options->Debugging->Automatically close the console when debugging stops.
    Press any key to close this window . . .
    
    