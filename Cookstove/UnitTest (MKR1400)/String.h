// -*- c++ -*-
#pragma once

#include <string>
#include <iostream>

class String : public std::string
{
public:
  String(std::string s):
    std::string(s) {}
  
  String(const char c):
    std::string(1, c) {}

  String(const char s[]):
    std::string(s) {}
  
  String(float f):
    std::string(std::to_string(f)) {}

  String(unsigned int f):
    std::string(std::to_string(f)) {}

};

void _PRINT_TIME(String msg) {
  std::cout << msg << std::endl;
}
