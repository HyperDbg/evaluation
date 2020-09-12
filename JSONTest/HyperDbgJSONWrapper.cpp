/**
 * @file HyperDbgJSONWrapper.cpp
 * @author M.H. Gholamrezei (gholamrezaei.mh@gmail.com)
 * @brief Implementation of JSON Converter 
 * @details
 * 
 * @version 0.1
 * @date 2020-12-9
 * 
 * @copyright This project is released under the GNU Public License v3.
 * 
 */
#include "JSON.h"
#include "HyperDbgJSONWrapper.h"
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<tuple>
#include<string.h>
#include<iostream>
#include<Windows.h>
using namespace std;

vector<tuple<vector<JSON_TOKEN_FORMAT>, UINT64>> FormatContainer;


/**
 * @brief Add a list of formats to the format container
 * 
 */
VOID JSONFormat(vector<JSON_TOKEN_FORMAT> &Format, UINT64 Tag)
{
    FormatContainer.push_back(make_tuple(Format, Tag));   
}


/**
 * @brief Converts a buffer filled with JSON_TOKENS to a JSON string
 * 
 */
VOID Convert2JSON(JSON_OBJECT* Root, UINT64 Tag, CHAR* Buffer, UINT32 MaxSize)
{
    vector<JSON_TOKEN_FORMAT> Format;

    //
    // Allocate memory for JSON string
    //
    CreateJsonString(Root, MaxSize);

    // 
    // Search for tag in Formats list 
    //
    Format = GetFormatByTag(Tag);
    
    
    UINT32 Indx = 0;
    struct JSON_TOKEN* Token;
    struct JSON_TOKEN_FORMAT TokenFormat;
    BOOL HasNext = true;

    //
    // Traverse buffer and append value of each Token to JSON string 
    //
    while(HasNext)
    {
        //
        // Extract Token from buffer
        //
        Token = (struct JSON_TOKEN*)((UINT64)Buffer+Indx);

        TokenFormat = GetTokenFormatById(&Format, Token->Id);

        //
        // Add Token to JSON Root Object
        //
        AddToken(Root, Token, &TokenFormat);

        //  
        // Update Index
        //
        Indx+= Token->Size; 


        //
        // Update HasNext by new Token
        //
        HasNext = Token->HasNext;

    }

    
    //
    // Closes JSON string
    //
    Close(Root);
}

/**
* @brief returns correspoing format list to the Tag
*
*/
vector<JSON_TOKEN_FORMAT> GetFormatByTag(UINT64 Tag)
{
    vector<JSON_TOKEN_FORMAT> Format;
    for (tuple<vector<JSON_TOKEN_FORMAT>,UINT64> f:FormatContainer)
    {
        if(get<1>(f) == Tag)
        {
            Format = get<0>(f);
        }
    }
    return Format;
}

/**
* @brief returns correspoing token format to a format list ID
*
*/
JSON_TOKEN_FORMAT GetTokenFormatById(vector<JSON_TOKEN_FORMAT>* Format,UINT32 Id)
{
    for(JSON_TOKEN_FORMAT t:*Format)
    {
        if(t.Id == Id)
        {
            return t;
        }
    }
}

/**
* @brief Adds a token to the JSON String. 
* @details This function handles various types of JSON_TOKEN
*       
*
*/
VOID AddToken(JSON_OBJECT* Root, JSON_TOKEN* Token, JSON_TOKEN_FORMAT* Format)
{
    switch(Format->TypeOfToken)
    {
        case(JSON_TOKEN_FORMAT::TYPE_ASCII):
        {
            char* value = (char*)((UINT64)Token + (sizeof(JSON_TOKEN)));
            AddString(Root, Format->Name, value);
            break;
        }
        case(JSON_TOKEN_FORMAT::TYPE_UNICODE):
        {
            wchar_t* value = (wchar_t*)((UINT64)Token + (sizeof(JSON_TOKEN)));
            AddWstring(Root, Format->Name, value);
            break;
        }
        
        case(JSON_TOKEN_FORMAT::TYPE_DOUBLE):
        {
            VOID* addr = (VOID*)((UINT64)Token + (sizeof(JSON_TOKEN)));
            DOUBLE x = *(DOUBLE*)addr;
            AddDouble(Root, Format->Name, x);    
            break;
        }
        case(JSON_TOKEN_FORMAT::TYPE_FLOAT):
        {
            VOID* addr = (VOID*)((UINT64)Token + (sizeof(JSON_TOKEN)));
            FLOAT x = *(FLOAT*)addr;
            AddFloat(Root, Format->Name, x);    
            break;
        }
        case(JSON_TOKEN_FORMAT::TYPE_INT32):
        {
            VOID* addr = (VOID*)((UINT64)Token + (sizeof(JSON_TOKEN)));
            INT32 x = *(INT32*)addr;
            AddInt32(Root, Format->Name, x);    
            break;
        }
        case(JSON_TOKEN_FORMAT::TYPE_UINT32):
        {
            VOID* addr = (VOID*)((UINT64)Token + (sizeof(JSON_TOKEN)));
            UINT32 x = *(UINT32*)addr;
            AddUint32(Root, Format->Name, x);    
            break;
        }
        case(JSON_TOKEN_FORMAT::TYPE_INT64):
        {
            VOID* addr = (VOID*)((UINT64)Token + (sizeof(JSON_TOKEN)));
            INT64 x = *(INT64*)addr;
            AddInt64(Root, Format->Name, x);    
            break;
        }
        case(JSON_TOKEN_FORMAT::TYPE_UINT64):
        {
            VOID* addr = (VOID*)((UINT64)Token + (sizeof(JSON_TOKEN)));
            UINT64 x = *(UINT64*)addr;
            AddUint64(Root, Format->Name, x);    
            break;
        }
    }

}
