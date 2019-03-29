#pragma once
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class FileReadUtil
{
private:
	static FileReadUtil * instance;
	ifstream readStream;
public:
	static FileReadUtil * GetInstance();
private:
	FileReadUtil() {};
	void readFromFilePath(string path,char *pst, int size);
};

