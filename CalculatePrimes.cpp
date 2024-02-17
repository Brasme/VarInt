#include "VarInt.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

string diffStr(const std::string &s1,const std::string & s2)
{
    stringstream ss;
    auto si1=s1.begin();
    auto si2=s2.begin();
    while (si1!=s1.end()) {
        if (si2!=s2.end()) {
            ss << ((*si1 != *si2)?'*':'-');
            ++si2;
        } else {
            ss << '^';
        }
        ++si1;
    }
    while (si2!=s2.end()) {
        ss << 'v';
        ++si2;
    }
    return ss.str();
}

void example() {
    VarUint v1(0xff);
	VarUint v2 = 0xef;

    cout << "v1=" << v1 << '(' << v1.ToDecStr() << "), bytes=" << v1.NumBytes() << endl;
    cout << "v2=" << v2 << '(' << v1.ToDecStr() << "), bytes=" << v2.NumBytes() << endl;

    VarUint v3 = v1 * v2;
    cout << "v3=v1*v2=" << v3 << "(==0xFE11?)" << '(' << v3.ToDecStr() << "), bytes=" << v3.NumBytes() << endl;

    VarUint v4 = v3 / v2;
    cout << "v4=v3/v2=" << v4 << "(==0xFF?)" << '(' << v4.ToDecStr() << "), bytes=" << v4.NumBytes() << endl;

    VarUint v5 = v3 * v3 * v3 * v2;
    cout << "v5=v3 * v3 * v3 * v2=" << v5 << "(==0xC03559674940BF?)" << '(' << v5.ToDecStr() << "), bytes=" << v5.NumBytes() << endl;

    VarUint v6 = v5 * v5 * v5 * v5;
    cout << "v6=v5^4=" << v6 << '(' << v6.ToDecStr() << "), bytes=" << v6.NumBytes() << endl;


    {
        string expected="0x515A2C6AF8EE07C49953E663D0DE040CB5C823F2A2621C5B356E5D01";
        string result=v6.ToHexStr();
        if (result!=expected)
            cout << "Missmatch hex:\n"
            " result = '" << result << "'\n"
            "       != '" << expected << "'\n";
    }
    
    // VarUint remain;
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // cout                     << __LINE__ << "v6/10 => " << "0x8229e0ab27e33fa0f5530a394e30067abc736cb76a3693c5224a2e6\n";

    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // cout                     << __LINE__ << "v6/10 => " << "0xd0430111d96b99018884dd287d19a3f793ebe12576bdb93b6a104a\n";

    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    // v6.Div(10,&remain); cout << __LINE__ << "v6/10 => " << v6 << ',' << remain << "\n";
    
    {
        string expected="8567390854443331688706960582058466394167047921945064008730712890625";
        string result=v6.ToDecStr();
        if (result!=expected)
            cout << "Missmatch dec:\n"
            " result = '" << result << "'\n"
            "       != '" << expected << "'\n"
            "       >> '" << diffStr(result,expected) << "'\n";
    }
}

int main() {
    cout << "Calculate prime statistics\n";    
    example();
    return 0;
}