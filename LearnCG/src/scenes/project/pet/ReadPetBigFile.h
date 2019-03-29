#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

#pragma pack(1) //关闭结构体对齐
struct SingleCoin { //12字节
	unsigned long long nTime; //8字节
	unsigned short CrystalId; //2字节
	unsigned char BDMId; //1字节
	unsigned char DUId; //1字节
}; 
// sizeof(SingleCoin)=16字节
//关闭结构体对齐之后 sizeof(SingleCoin)=12字节
#pragma pack()

class ReadPetBigFile
{
public:
	ifstream readStream;
	string petDataFilename;
	//vector<SingleCoin> coins
	SingleCoin coins[617880];
	int coinCount = 617880; // ÷2 对
public:
	ReadPetBigFile();
	~ReadPetBigFile();
	void writeSingleCoin();
	void coutSingleCoin(SingleCoin &s);
};

