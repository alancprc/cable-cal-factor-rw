#include "util.h"

using namespace std;

std::string removeWhiteSpace(const std::string& str)
{
  std::string newStr(str);
  newStr.erase(0, newStr.find_first_not_of(" \t\r\n"));
  newStr.erase(newStr.find_last_not_of(" \t\r\n") + 1);
  return newStr;
}

