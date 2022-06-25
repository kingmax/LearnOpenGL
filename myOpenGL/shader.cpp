#include "shader.h"

#pragma region ERROR Function
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

const string readVS(const string vsFilename)
{
	ifstream inFile(vsFilename);
	if (!inFile)
	{
		cerr << "Unable to open file" << endl;
		return "";
	}

	stringstream ss;
	string line;
	while (getline(inFile, line))
	{
		ss << line << "\n";
	}
	inFile.close();
	line = ss.str();
	//cout << line << endl;
	return line;
}
#pragma endregion ERROR Function

void readShaderFile(const string vsFilename, string& outString)
{
	ifstream inFile(vsFilename);
	if (!inFile)
	{
		cerr << "Unable to open file" << endl;
		return;
	}

	stringstream ss;
	string line;
	while (getline(inFile, line))
	{
		ss << line << "\n";
	}
	inFile.close();
	
	outString = ss.str();
}