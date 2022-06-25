#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
using namespace std;

//const char* readVS(const char* vsFilename);
//const string readVS(const string vsFilename);
void readShaderFile(const string vsFilename, string& outString);