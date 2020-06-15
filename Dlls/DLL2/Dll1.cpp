#include "pch.h"
#include "Dll1.h"
#include <functional>
#include <iostream>

bool TestClass1::test()
{
	std::bad_function_call e;
	std::cout << "TestBadFunctionCall \n";
	std::function<int()> TestBadFunction = nullptr;
	TestBadFunction();
	throw e;
	return true;
}
