// stdafx.cpp : source file that includes just the standard includes
// testFastDB2.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

char* allocString(string value)
{
    char* str = new char[value.length()+1];
    strcpy(str, value.c_str());
    return str;
}

void stringSetter(const char** property, string value)
{
    clearStringProperty(property);
    *property = const_cast<const char*>(allocString(value));
}

char const* stringGetter(char const* property)
{
    return property == NULL ? "" : property;
}

void clearStringProperty(const char** property)
{
    if (*property != NULL)
    {
        delete *property;
    }
}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

string join(vector<string>& str_arr, string delim) {
    if (str_arr.size() == 0)
    {
        return "";
    }
    string ret = str_arr[0];
    for (size_t i=1; i<str_arr.size(); i++)
    {
        ret = ret + delim + str_arr[i];
    }

    return ret;
}
