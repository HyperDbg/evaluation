/**
 * @file HyperDbgJSONWrapper.h
 * @author M.H. Gholamrezei (gholamrezaei.mh@gmail.com)
 * @brief JSON Wrapper Header File
 * @details
 * 
 * @version 0.1
 * @date 2020-12-9
 * 
 * @copyright This project is released under the GNU Public License v3.
 * 
 */
#ifndef HYPERDBGJSONWRAPPER_H
#define HYPERDBGJSONWRAPPER_H
#include "JSON.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <tuple>
#include <string.h>
#include <iostream>
#include <Windows.h>
using namespace std;
/*
    TODO:
        5.  Clean up code
        6.  Apply Windows Convention 
        7.  Doxygen comments

*/

struct JSON_TOKEN_FORMAT
{
    UINT32 Id;
    enum {TYPE_ASCII, TYPE_FLOAT, TYPE_DOUBLE, TYPE_UINT64, TYPE_INT64, TYPE_INT32, TYPE_UINT32, TYPE_UNICODE} TypeOfToken;
    wchar_t* Name; 
};

struct JSON_TOKEN
{
    UINT32 Id; 
    UINT32 Size;
    BOOL HasNext;
};

VOID JSONFormat(vector<JSON_TOKEN_FORMAT> &Format, UINT64 Tag);
VOID Convert2JSON(JSON_OBJECT* Root, UINT64 Tag, CHAR* buffer, UINT32 MaxSize);

VOID PrintFormat(vector<JSON_TOKEN_FORMAT> &Format);

vector<JSON_TOKEN_FORMAT> GetFormatByTag(UINT64 Tag);

JSON_TOKEN_FORMAT GetTokenFormatById(vector<JSON_TOKEN_FORMAT>* Format,UINT32 Id);
VOID AddToken(JSON_OBJECT* Root, JSON_TOKEN* Token, JSON_TOKEN_FORMAT* Format);
#endif