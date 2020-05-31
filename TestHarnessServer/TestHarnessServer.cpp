/*
 *-------------------------------------
 * Barry L Armour, Huan Doan, Victoria Tenney, Yuexin Yu
 * CSE 687 Object Oriented Design C++
 * Syracuse University
 * Design Project: Test Harness
 *-------------------------------------
 * Implemented member functions in TestHarness class.
 * Defined the entry point for TestHarness package.
 * TestHarness server spawnes Client process and Child
 * processes by using the Process poll framework provided
 * by Professor, configuration data, like ip address, port
 * number, server name and file pathes, are sent by Testness
 * server to each child processes.
 */

#include "TestHarnessServer.h"
#include "../SupportLibrary/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../SupportLibrary/FileSystem/FileSystem.h"
#include "../SupportLibrary/Utilities/FileUtilities.h"
#include <thread>
#include <chrono>

#define TEST_TESTHARNESS

namespace Testing
{
	// Constructor for TestHarnessServer, require lib MsgPassingComm: EndPoint, comm_, from_
	TestHarnessServer::TestHarnessServer(EndPoint from)
		: comm_(from, "recvr"), from_(from)
	{
		comm_.start();
		start();
	}

	// Spawn client process
	void TestHarnessServer::start()
	{
		std::string ChildPath = "../Debug/ChildTester.exe";

		//Spawn Child Processes
		Process p1;
		p1.application(ChildPath);

		Process p2;
		p2.application(ChildPath);

		Process p3;
		p3.application(ChildPath);

		std::ostringstream os;
		os << from_.address << " " << from_.port << " " << "Child_Tester_1 " << "localhost " << "8001 " << "../SendFile ../SaveFile";
		std::string cmdLine_Child_1 = os.str();
		os.str("");

		os << from_.address << " " << from_.port << " " << "Child_Tester_2 " << "localhost " << "8002 " << "../SendFile ../SaveFile";
		std::string cmdLine_Child_2 = os.str();
		os.str("");

		os << from_.address << " " << from_.port << " " << "Child_Tester_3 " << "localhost " << "8003 " << "../SendFile ../SaveFile";
		std::string cmdLine_Child_3 = os.str();
		os.str("");

		p1.title("Child_Tester_1");
		p2.title("Child_Tester_2");
		p3.title("Child_Tester_3");

		p1.commandLine(cmdLine_Child_1);
		p2.commandLine(cmdLine_Child_2);
		p3.commandLine(cmdLine_Child_3);

		p1.create();
		p2.create();
		p3.create();

		std::function<void(void)> callback = []()
		{
			std::cout << "\n\n-- A Child Tester Has Exited...";
		};

		p1.setCallBackProcessing(callback);
		p1.registerCallback();
		p2.setCallBackProcessing(callback);
		p2.registerCallback();
		p3.setCallBackProcessing(callback);
		p3.registerCallback();

		std::cout << "\n\n-- Starting TestHarness Server Receiving Thread Listening On: " << from_.toString();
		recvr = std::thread([&]() { recvMessages(); });
		recvr.detach();

		std::cout << "\n\n-- Starting TestHarness Server Dispatching Thread:";
		dspat = std::thread([&]() { dispatchMessages(); });
		dspat.detach();
	}

	// Wait for TestHarness Server to shutdow
	void TestHarnessServer::wait()
	{
		if (recvr.joinable())
			recvr.join();
		if (dspat.joinable())
			dspat.join();
	}

	// Stop TestHarness server 
	void TestHarnessServer::stop()
	{
		MsgPassingCommunication::Message stop;
		stop.command("TestHarnessStop");
		stop.to(from_);  // send to self
		stop.from(from_);
		comm_.postMessage(stop);
	}

	// Send a message anywhere you want 
	void TestHarnessServer::sendMsg(MsgPassingCommunication::Message msg)
	{
		comm_.postMessage(msg);
	}

	// Revceive message 
	void TestHarnessServer::recvMessages()
	{
		while (true)
		{
			MsgPassingCommunication::Message msg = comm_.getMessage();
			std::cout << "\n\n-- Test_Harness_Server Has Received Messages: " + msg.name();

			if (msg.command() == "ready")
			{
				std::cout << "\n\n-- Test_Harness_Server enqueues a Ready_Message From A Child_Tester..";
				readyQ_.enQ(msg);
			}
			else if (msg.command() == "testReq")
			{
				std::cout << "\n\n-- Test_Harness_Server enqueues a Test_Request_Message From GUI_Client..";
				requestQ_.enQ(msg);
			}
			else if (msg.command() == "stop")
			{
				std::cout << "\n\n-- Test_Harness_Server enqueues Stop_Testing_Message..";
				requestQ_.enQ(msg);
			}
			else
			{
				std::cout << "\n\n-- Test_Harness_Server has received an invalid message command.." << msg.command();
				std::cout << "\n-- Message title: " << msg.name();
			}
		}
	}


	// Dequeue messages and send to testers 
	void TestHarnessServer::dispatchMessages()
	{
		while (true)
		{
			MsgPassingCommunication::Message trMsg = requestQ_.deQ();
			std::cout << "\n-- Test_Harness_Server_Dispatch deQ Request_Message: " + trMsg.command();

			MsgPassingCommunication::Message rdyMsg = readyQ_.deQ();
			std::cout << "\n-- Test_Harness_Server_Dispatch deQ Ready_Message: " + rdyMsg.command();

			// Sending a request to ready child
			trMsg.to(rdyMsg.from());  
			std::cout << "\n-- Test_Harness_Server_Dispatch Sending Message to Child Tester: " + trMsg.command();
			comm_.postMessage(trMsg);
		}
	}

	//----< Setup file sending and receiving path >------------------------
	void TestHarnessServer::setPath(std::string& send_path, std::string& save_path)
	{
		comm_.setSendFilePath(send_path);
		comm_.setSaveFilePath(save_path);
	}

	//----< Get file send path >------------------------
	std::string TestHarnessServer::getSendPath()
	{
		return comm_.getSendFilePath();
	}

	//----< Get file receive path >------------------------
	std::string TestHarnessServer::getSavePath()
	{
		return comm_.getSaveFilePath();
	}
}

#ifdef TEST_TESTHARNESS

int main()
{
	std::cout << "------------  Welcome to Test Harness Server  ------------" << std::endl;

	// Set Test Harness Server Port to 8999; GUI Client Port is 8888
	EndPoint from("localhost", 8888);
	Testing::TestHarnessServer th(from);

	std::string send_path = "../SendFile";
	std::string save_path = "../SaveFile";
	th.setPath(send_path, save_path);

	std::cout << "\n\n----------------------------------------------------------" << std::endl;
	std::cout << "-- Test Server Configuration: " << std::endl;
	std::cout << "-- Network Domain: " << "Test_Harness_Server" << std::endl;
	std::cout << "-- Port Address: " << 8888 << std::endl;
	std::cout << "-- Test Driver Send Path: " << send_path << std::endl;
	std::cout << "-- Test Driver Save Path: " << save_path << std::endl;

	getchar();
	return 0;
}

#endif