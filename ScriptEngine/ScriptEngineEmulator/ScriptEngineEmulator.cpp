/**
 * @file print.cpp
 * @author Sina Karvandi (sina@rayanfam.com)
 * @brief print command
 * @details
 * @version 0.1
 * @date 2020-10-08
 *
 * @copyright This project is released under the GNU Public License v3.
 *
 */
#include <string>
#include <Windows.h>

using namespace std;

extern "C" {
    __declspec(dllimport) void ScriptEngineParse(char* str);
}

VOID TestParser(string Expr);
////////////////////////////////////////////////////////////////////
//                  Test (Should be removed)
////////////////////////////////////////////////////////////////////

typedef unsigned long long QWORD;

#define LOWORD(l) ((WORD)(l))
#define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w) ((BYTE)(w))
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))

typedef struct _GUEST_REGS {
    ULONG64 rax; // 0x00
    ULONG64 rcx; // 0x08
    ULONG64 rdx; // 0x10
    ULONG64 rbx; // 0x18
    ULONG64 rsp; // 0x20
    ULONG64 rbp; // 0x28
    ULONG64 rsi; // 0x30
    ULONG64 rdi; // 0x38
    ULONG64 r8;  // 0x40
    ULONG64 r9;  // 0x48
    ULONG64 r10; // 0x50
    ULONG64 r11; // 0x58
    ULONG64 r12; // 0x60
    ULONG64 r13; // 0x68
    ULONG64 r14; // 0x70
    ULONG64 r15; // 0x78
} GUEST_REGS, * PGUEST_REGS;

//
// Pseudo registers
//

// $tid
UINT64 ScriptEnginePseudoRegGetTid() { return GetCurrentThreadId(); }

// $pid
UINT64 ScriptEnginePseudoRegGetPid() { return GetCurrentProcessId(); }

//
// Keywords
//

// poi
UINT64 ScriptEngineKeywordPoi(PUINT64 Address) { return *Address; }

// hi
WORD ScriptEngineKeywordHi(PUINT64 Address) {
    QWORD Result = *Address;
    return HIWORD(Result);
}

// low
WORD ScriptEngineKeywordLow(PUINT64 Address) {
    QWORD Result = *Address;
    return LOWORD(Result);
}

// db
BYTE ScriptEngineKeywordDb(PUINT64 Address) {
    BYTE Result = *Address;
    return Result;
}

// dd
WORD ScriptEngineKeywordDd(PUINT64 Address) {
    WORD Result = *Address;
    return Result;
}

// dw
DWORD ScriptEngineKeywordDw(PUINT64 Address) {
    DWORD Result = *Address;
    return Result;
}

// dq
QWORD ScriptEngineKeywordDq(PUINT64 Address) {
    QWORD Result = *Address;
    return Result;
}

//
// test function
//

VOID PerformAction(PGUEST_REGS GuestRegs, string Expr) {

    //
    // Test Parser
    //
    ScriptEngineParse((char*)Expr.c_str());
    /*for(token in buffer )
        type = command.type 
        value = command.value */

        
    
}

VOID TestParser(string Expr) {

    typedef struct _TEST_STRUCT {
        UINT64 Var1;
        UINT64 Var2;
        UINT64 Var3;
        UINT64 Var4;
    } TEST_STRUCT, * PTEST_STRUCT;

    PTEST_STRUCT TestStruct = (PTEST_STRUCT)malloc(sizeof(TEST_STRUCT));
    RtlZeroMemory(TestStruct, sizeof(TEST_STRUCT));

    TestStruct->Var1 = 0x41414141;
    TestStruct->Var3 = 0x4242424242424242;

    GUEST_REGS GuestRegs = { 0 };

    GuestRegs.rax = 0x1;
    GuestRegs.rcx = (UINT64)TestStruct;
    GuestRegs.rdx = 0x3;
    GuestRegs.rbx = 0x4;
    GuestRegs.rsp = 0x5;
    GuestRegs.rbp = 0x6;
    GuestRegs.rsi = 0x7;
    GuestRegs.rdi = 0x8;
    GuestRegs.r8 = 0x9;
    GuestRegs.r9 = 0xa;
    GuestRegs.r10 = 0xb;
    GuestRegs.r11 = 0xc;
    GuestRegs.r12 = 0xd;
    GuestRegs.r13 = 0xe;
    GuestRegs.r14 = 0xf;
    GuestRegs.r15 = 0x10;

    PerformAction(&GuestRegs, Expr);
}


int main() 
{
    // string str = "x = poi(0n100); ";
    // string str = "SINA=dw(1+(1*5)+(34*34)|7*(99^34) ) ; \n x = str(dq(dw( dd(db(poi(wstr(34)&sizeof(1)>>not(2)<<neg(3)+hi(4)-low(5)*6/7%8)))))); \n";
    string str = "x1 = poi(poi((poi(($proc&neg(1000`0000))+10)^poi (poi(poi(poi(poi(poi(poi(poi($prcb+18)+220)+648)+8)-240)+2a0)))^neg(0n6708588087252463955)^($proc&neg(100000)))-D8)-1080); ";
    TestParser(str.c_str());
    return 0;
}