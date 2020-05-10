/////////////////////////////////////////
// Huan Doan 2020/05/02
// CS87 bject orriented Design
// Virtual Class ITest to setup interface for test DLL 
// The interface has one functon with no argument and will return boo; 

#ifndef ITEST_H
#define ITEST_H

#ifdef TEST_EXPORTS
#define TEST_API __declspec(dllexport)
#else
#define TEST_API __declspec(dllimport)
#endif

class  ITest
{
public:
	virtual ~ITest() {};
	//virtual bool test() = 0;
	virtual bool operator()() = 0;
};
#endif
