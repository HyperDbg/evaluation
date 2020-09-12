/**
 * @file main.cpp
 * @author M.H. Gholamrezei (gholamrezaei.mh@gmail.com)
 * @brief Test of JSON Wrapper
 * @details
 * 
 * @version 0.1
 * @date 2020-12-9
 * 
 * @copyright This project is released under the GNU Public License v3.
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <tuple>
#include <string.h>
#include <iostream>
#include <Windows.h>
#include "HyperDbgJSONWrapper.h"
using namespace std;

#pragma warning(disable : 4996)

char* Append(char* Buffer, JSON_TOKEN* Token, void* Value)
{
    memcpy(Buffer, (VOID*)Token, sizeof(JSON_TOKEN));
    UINT64 ValueSize = (UINT64)Token->Size - (UINT64)sizeof(JSON_TOKEN);
    memcpy((VOID*)((UINT64)Buffer+(UINT64)sizeof(JSON_TOKEN)), (VOID*)(Value), ValueSize);
    
    Buffer = Buffer + Token->Size;
    return Buffer;
}

int main()
{
    char* Buffer;
    char* Pointer;
    Pointer = Buffer = (char*)malloc(100);
    RtlZeroMemory(Buffer, 100);
    JSON_OBJECT Root;

    //
    // Fill the format within a sample format
    // 
    vector<JSON_TOKEN_FORMAT> FormatList;

    struct JSON_TOKEN_FORMAT Format1= {0, JSON_TOKEN_FORMAT::TYPE_DOUBLE, (wchar_t*)L"Age" };
    FormatList.push_back(Format1);
    
    struct JSON_TOKEN_FORMAT Format2= {1, JSON_TOKEN_FORMAT::TYPE_ASCII, (wchar_t*)L"Gender"};
    FormatList.push_back(Format2);

    struct JSON_TOKEN_FORMAT Format3= {2, JSON_TOKEN_FORMAT::TYPE_UNICODE, (wchar_t*)L"Favorite Food"};
    FormatList.push_back(Format3);
    
    UINT64 tag1 = 1;
    JSONFormat(FormatList, tag1);

    
    DOUBLE x = 18.0;
    VOID* MyVal1 = (VOID*)&x;
    struct JSON_TOKEN Token1 = {0, sizeof(DOUBLE) +sizeof(JSON_TOKEN), true};
    Pointer = Append(Pointer, &Token1, MyVal1);
    
    char* myVal2 = (char*)"Male";
    struct JSON_TOKEN Token2 = {1, strlen(myVal2)+1 + sizeof(JSON_TOKEN), true};
    Pointer = Append(Pointer, &Token2, (void*)myVal2);

    wchar_t* myVal3 = (wchar_t*)L"Ghormeh Sabzi";
    struct JSON_TOKEN Token3 = {2, wcslen(myVal3)*sizeof(wchar_t)+2 + sizeof(JSON_TOKEN), false};
    Pointer = Append(Pointer, &Token3, (void*)myVal3);

    Convert2JSON(&Root, tag1, Buffer, 1000);
    wprintf(L"%s\n",Root.Base);

    return 0; 
}
