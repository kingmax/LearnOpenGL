#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>

using namespace std;

const char* readVS(const char* vsFilename)
{
	ifstream inFile(vsFilename);
	//inFile.open(vsFilename);
	if (!inFile)
	{
		cerr << "Unable to open file" << endl;
		return "";
	}

	stringstream ss;
	string line;
	//while (inFile >> line)
	while(getline(inFile, line))
	{
		ss << line << endl;
	}
	inFile.close();
	cout << ss.str() << endl;
	return ss.str().c_str();
}