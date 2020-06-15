#ifdef DLL1_EXPORTS
#define DLL1_API __declspec(dllexport)
#else
#define DLL1_API __declspec(dllimport)
#endif

#include "Itest.h"

class TestClass1 : public ITest {

public:
	bool test() override;
};


extern "C" DLL1_API TestClass1  *createTest() {
	return new TestClass1;
};
