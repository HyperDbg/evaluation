/**
 * @file JSON.h
 * @author M.H. Gholamrezei (gholamrezaei.mh@gmail.com)
 * @brief JSON Header File
 * @details
 * 
 * @version 0.1
 * @date 2020-12-9
 * 
 * @copyright This project is released under the GNU Public License v3.
 * 
 */
#ifndef JSON_H__
#define JSON_H__
#include <wchar.h>
#include <stdint.h>
#include <Windows.h>

typedef struct JSON_OBJECT
{
    wchar_t* Base;
    wchar_t* Pointer;
    UINT32   Capacity;
} JSON_OBJECT;

VOID CreateJsonString(JSON_OBJECT* Obj, INT32 MaxSize); 
VOID AddInt32(JSON_OBJECT* Obj, const wchar_t* Name, INT32 x);
VOID AddUint32(JSON_OBJECT* Obj, const wchar_t* Name, UINT32 x);
VOID AddInt64(JSON_OBJECT* Obj, const wchar_t* Name, INT64 x);
VOID AddUint64(JSON_OBJECT* Obj, const wchar_t* Name, UINT64 x);
VOID AddDouble(JSON_OBJECT* Obj, const wchar_t* Name, DOUBLE x);
VOID AddFloat(JSON_OBJECT* Obj, const wchar_t* Name, FLOAT x);
VOID AddString(JSON_OBJECT* Obj, const wchar_t* Name, CHAR* x);
VOID AddWstring(JSON_OBJECT* Obj, const wchar_t* Name, wchar_t* x);
wchar_t* getString(JSON_OBJECT* Obj);
VOID Close(JSON_OBJECT* Obj);


#endif