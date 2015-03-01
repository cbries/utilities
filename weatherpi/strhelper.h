/*
 */

#pragma once

#ifndef __STRHELPER_H__
#define __STRHELPER_H__

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#include <string>
#include <vector>

namespace Helper
{
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
std::string &ltrim(std::string &s);
std::string &rtrim(std::string &s);
std::string &trim(std::string &s);
} // namespace Helper

#endif // __STRHELPER_H__

