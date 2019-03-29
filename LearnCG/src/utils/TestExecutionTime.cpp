#include "TestExecutionTime.h"


clock_t TestExecutionTime::start_time = 0;
clock_t TestExecutionTime::end_time = 0;

void TestExecutionTime::start()
{
	start_time = clock();
}

void TestExecutionTime::end()
{
	end_time = clock();
	std::cout << "Running time is: " 
			<< static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;//输出运行时间
}

TestExecutionTime::TestExecutionTime()
{
}


TestExecutionTime::~TestExecutionTime()
{
}
