#include "pch.h"
#include "Dll1.h"
#include <functional>
#include <iostream>

bool TestClass1::test()
{
	std::bad_weak_ptr e;
	std::cout << "TestBadWeakPtr \n";
	std::shared_ptr<int> p1(new int(100));
	std::weak_ptr<int> wp(p1);
	p1.reset();
	throw e;
	return true;
}
