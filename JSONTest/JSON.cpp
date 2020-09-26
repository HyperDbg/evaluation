/**
 * @file JSON.cpp
 * @author M.H. Gholamrezei (gholamrezaei.mh@gmail.com)
 * @brief Implementation of TOKEN buffer to JSON Converter 
 * @details
 * 
 * @version 0.1
 * @date 2020-12-9
 * 
 * @copyright This project is released under the GNU Public License v3.
 * 
 */
#include "Json.h"
#include <wchar.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>


/**
 * @brief Allocate memory for JSON string
 * 
 */
VOID CreateJsonString(JSON_OBJECT* Obj, INT MaxSize)
{
    Obj->Base=(wchar_t*)malloc(MaxSize * sizeof(wchar_t));
    Obj->Pointer = Obj->Base;
    Obj->Capacity = MaxSize;
}

/**
 * @brief Adds an INT32 variable to JSON String
 * 
 */
VOID AddInt32(JSON_OBJECT* Obj, const wchar_t* Name, INT32 x)
{
    
    int Len = swprintf(Obj->Pointer, Obj->Capacity, L"\t\"%s\": %d\n},\n", Name, x);
    Obj->Pointer += Len;
    Obj->Capacity -= Len;
}

/**
 * @brief Adds a UINT32 variable to JSON String
 * 
 */
VOID AddUint32(JSON_OBJECT* Obj, const wchar_t* Name, UINT32 x)
{
    int Len = swprintf(Obj->Pointer, Obj->Capacity,L"\t\"%s\": %u\n", Name, x);
    Obj->Pointer += Len;
    Obj->Capacity -= Len;
}

/**
 * @brief Adds an INT64 variable to JSON String
 * 
 */
VOID AddInt64(JSON_OBJECT* Obj, const wchar_t* Name, INT64 x)
{
    int Len = swprintf(Obj->Pointer, Obj->Capacity, L"\t\"%s\": %lld\n", Name, x);
    Obj->Pointer += Len;
    Obj->Capacity -= Len;
}

/**
 * @brief Adds a UINT64 variable to JSON String
 * 
 */
VOID AddUint64(JSON_OBJECT* Obj, const wchar_t* Name, UINT64 x)
{
    int Len = swprintf(Obj->Pointer, Obj->Capacity, L"\t\"%s\": %llu\n", Name, x);
    Obj->Pointer += Len;
    Obj->Capacity -= Len;
}

/**
 * @brief Adds a DOUBLE variable to JSON String
 * 
 */
VOID AddDouble(JSON_OBJECT* Obj, const wchar_t* Name, DOUBLE x)
{
    int Len = swprintf(Obj->Pointer, Obj->Capacity, L"\t\"%s\": %lf\n", Name, x);
    Obj->Pointer += Len; 
    Obj->Capacity -= Len;
}

/**
 * @brief Adds a FLOAT variable to JSON String
 * 
 */
VOID AddFloat(JSON_OBJECT* Obj, const wchar_t* Name, FLOAT x)
{
    int Len = swprintf(Obj->Pointer, Obj->Capacity, L"\t\"%s\": %f\n", Name, x);
    Obj->Pointer += Len;
}

/**
 * @brief Adds an ASCII string to JSON String
 * 
 */
VOID AddString(JSON_OBJECT* Obj, const wchar_t* Name, CHAR* x)
{
  
    size_t StringLen = strlen(x) * sizeof(char);
    wchar_t* Wstring = (wchar_t*)malloc(StringLen*2+2);
    size_t WstringLen;
    mbstowcs_s(&WstringLen, Wstring, StringLen+1, x, StringLen);
    int Len = swprintf(Obj->Pointer, Obj->Capacity, L"\t\"%s\": \"%s\"\n", Name, Wstring);
    Obj->Pointer += Len;
    Obj->Capacity -= Len;
    free(Wstring);
}

/**
 * @brief Adds a UNICODE String to JSON String
 * 
 */
VOID AddWstring(JSON_OBJECT* Obj, const wchar_t* Name, wchar_t* x)
{
    int Len = swprintf(Obj->Pointer, Obj->Capacity, L"\t\"%s\": \"%s\"\n", Name, x);
    Obj->Pointer += Len;
    Obj->Capacity -= Len;
}

/**
 * @brief 
 * 
 * @return wchart_t* returns JSON as a unicode string
 */
wchar_t* getString(JSON_OBJECT* Obj)
{
    return Obj->Base;
}

/**
 * @brief Closes Obj
 * 
 */
VOID Close(JSON_OBJECT* Obj)
{
    int Len = swprintf(Obj->Pointer, Obj->Capacity, L"}\n");
    Obj->Pointer += Len;
    Obj->Capacity -= Len;
}

/**
 * @brief Opens Obj
 *
 */
VOID Open(JSON_OBJECT* Obj)
{
    int Len = swprintf(Obj->Pointer, Obj->Capacity, L"{\n");
    Obj->Pointer += Len;
    Obj->Capacity -= Len;
}
