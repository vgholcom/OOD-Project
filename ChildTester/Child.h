///////////////////////////////////////////////////////////////////
// Child Tester class
// - Loads TestRequests from requestPath.
//   Each TestRequest provides names of Dlls to load and test
// - Loads Test Dynamic Link Libraries (Dlls) from libPath.
// - Executes tests defined in each Dll.
// - Each tests logs ouputs using Tester class's logger
//////////////////////////////////////////////////////////////////
/*
 *-------------------------------------
 * Yuexin Yu
 * CSE 687 Object Oriented Design C++
 * Syracuse University
 * Design Project: Test Harness
 * yyu100@syr.edu
 *
 * Huan Doan
 *-------------------------------------
 * This package defines one class, Tester:
 * - Uses DllLoader to load libraries and extract tests.
 * - Provides pointer to its member logger to each test, then
 * - Executes it, using the infrastructure from TestUtilities.
*/

#ifndef TESTER_H
#define TESTER_H
#endif

#include "../SupportLibrary/MsgPassingComm/CommHeader.h"
#include "../SupportLibrary/TestRequest/TestRequest.h"
#include "../SupportLibrary/TestRequest/Properties.h"
#include "../SupportLibrary/Loader/DllLoader.h"
#include "../SupportLibrary/Logger/SingletonLoggerFactory.h"
#include <iostream>
#include <vector>
#include <string>

using namespace MsgPassingCommunication;

namespace Testing
{

	using Requests = std::vector<TestRequest>;
	using LoggerFactory = Utilities::SingletonLoggerFactory<1, Utilities::NoLock>;

	class ChildTester
	{

	public:
		ChildTester(std::string name, std::string &address, size_t &port, std::string &to_address, size_t &to_port);
		~ChildTester();

		void start();
		void wait();
		void doTest();
		void setPath(std::string& path);
		void sendMsg(MsgPassingCommunication::Message &msg);
		void setSaveFilePath(std::string &save_path);
		void setSendFilePath(std::string &send_path);

		Property<Requests> requests;
		Property<std::string> dllPath;

	private:
		std::thread recv;
		std::string name_;
		std::string path_;
		LoggerFactory* logger_factory;
		std::ofstream log_file_;
		EndPoint from_;
		EndPoint to_;
		Comm childComm_;
	};
}
