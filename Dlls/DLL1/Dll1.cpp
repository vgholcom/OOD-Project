#include "pch.h"
#include "Dll1.h"
#include <new>
#include <iostream>

bool TestClass1::test()
{
	std::bad_alloc x;
	std::cout << "TestBadAlloc \n";

	while (true)
	{
		new int[100000000ul];
	}

	throw x;

	return true;
}
