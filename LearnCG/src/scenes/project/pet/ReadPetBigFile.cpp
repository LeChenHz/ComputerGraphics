#include "ReadPetBigFile.h"
#include "../../../utils/TestExecutionTime.h"

ReadPetBigFile::ReadPetBigFile()
{
	petDataFilename = "E:/Desktop/PET/coin.dat";
	readStream = ifstream(petDataFilename, ifstream::binary);

	if (!readStream)
	{
		cout << "打开" << petDataFilename << "文件失败" << endl;
		return;
	}

	TestExecutionTime::start();
	SingleCoin s;
	writeSingleCoin();
	TestExecutionTime::end();
}



ReadPetBigFile::~ReadPetBigFile()
{
}

void ReadPetBigFile::writeSingleCoin()
{
	//readStream.read((char *)&s.nTime, sizeof(s.nTime));
	//readStream.read((char *)&s.CrystalId, sizeof(s.CrystalId));
	//readStream.read((char *)&s.BDMId, sizeof(s.BDMId));
	//readStream.read((char *)&s.DUId, sizeof(s.DUId));
	readStream.read((char *)coins, sizeof(SingleCoin) * coinCount);
}

void ReadPetBigFile::coutSingleCoin(SingleCoin & s)
{
	//cout << s.BDMId << endl;
	cout << s.nTime << "  " << s.CrystalId << "  " << (int)s.BDMId << "  " << (int)s.DUId << endl;
}
