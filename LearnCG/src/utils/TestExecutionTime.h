#pragma once

#include<iostream>
#include<time.h>

class TestExecutionTime
{
public:
	static void start();
	static void end();
	static clock_t start_time;
	static clock_t end_time;
public:
	TestExecutionTime();
	~TestExecutionTime();
};

