/**
* \file utils.cpp
* \author max
* Created on Tue Mar 23 22:28:46 2021
*/

#include "utils.h"
#include <sstream>


std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}


