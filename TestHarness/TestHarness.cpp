///////////////////////////////////////////////////////////////////////
// TestHarness.cpp                                                   //
// ver 1.0                                                           //
// Victoria Tenney, CSE687 - Object Oriented Design, Spring 2018     //
///////////////////////////////////////////////////////////////////////

#include "TestHarness.h"

// constructor 
TestHarness::TestHarness( void )
{
	epPeer1 = new MsgPassingCommunication::EndPoint("localhost", hostPort_);
	comm = new MsgPassingCommunication::Comm(*epPeer1, "Peer1");
	comm->start();

	// launch N tester processes
    int N(1); // Number of processes to launch
    createChildProcesses(N);
}

void TestHarness::createChildProcesses(int N)
{
	// TODO: 
	// -FOR N

	TestProcess tp1(1, 8082, hostPort_);
	epPeer2 = tp1.getEndPoint();

	//TestProcess tp2(8083, hostPort_);
	//epPeer3 = tp2.getEndPoint();
}

// receives messages
void TestHarness::recvMessage( void )
{
	MsgPassingCommunication::Message msg;
	while (true)
	{
		msg = comm->getMessage();
		
		if (msg.command() == "ready")
		{
			std::cout << "Recieved ready message." << std::endl;
			int pid = Utilities::Converter<size_t>::toValue(msg.procid());
			readyMsgQ_.enQ(pid);
		}
		else if (msg.command() == "testReq")
		{
			// TODO: parse out each dll in test request and enqueue
			// vector <struct{ string, int}> dlls; 
			//for auto dll : dlls
			//testRequestQ_.enQ(dll);
		}
		else if (msg.command() == "stop")
		{
			// TODO: send stop signal to all processes
			break;
		}
		else {
			std::cout << "Received message with invalid command: " << msg.command() << std::endl;
		}
	}
}

// sends tests to tester processes
void TestHarness::testHarnessExec( void )
{
	while (true)
	{
		//std::cout << "In testHarnessExec()..." << std::endl;

		// if ready messages and test requests exist
		while ( readyMsgQ_.size() > 0 )//&& testRequestQ_.size() > 0)
		{
			int pid = readyMsgQ_.deQ();

			//std::string testRequest = testRequestQ_.deQ();
			std::cout << "Sending to pid# " << std::to_string(pid).c_str() << std::endl;

			// TODO: send to tester process
			/*
			MsgPassingCommunication::Message msg;
			msg.to(*epPeer2);
			msg.from(*epPeer1);
			for (size_t i = 0; i < 25; ++i)
			{
				std::string countStr = Utilities::Converter<size_t>::toString(i);
				msg.name("msg" + countStr);
				std::cout << "\n  Peer1 posted message \"" << msg.name() << "\"";
				comm->postMessage(msg);
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
			*/
		}
	}
}

// runs test harness threads
void TestHarness::run( void )
{

	// thread to get messages
	std::cout << "Starting recvMessage thread." << std::endl;
	std::thread t1 = std::thread([&]() { recvMessage( ); });
	// create child tester threads & process messages
	std::cout << "Starting testHarnessExec thread." << std::endl;
	std::thread t2 = std::thread([&]() { testHarnessExec(); });

	// start threads
	t1.join();
	t2.join();

}

void TestHarness::addToTestRequestQ(std::vector<std::string> dlls)
{
	for (auto dll : dlls)
	{
		testRequestQ_.enQ(dll);
	}
}

void TestHarness::addToReadyQ(uint8_t pid)
{
	readyMsgQ_.enQ(pid);
}

// ---------------------------------------
// test the TestHarness class
int main()
{
	TestHarness th;
	
	// load string to testRequestQ
	std::vector<std::string> test_vector;
	test_vector.push_back("\\file\path\to\dll.dll");
	test_vector.push_back("\\another\path\to\dll.dll");
	test_vector.push_back("\\and\another\test\path\to\dll.dll");
	th.addToTestRequestQ(test_vector);

	// load PIDs to readyMessageQ
	//th.addToReadyQ(0);
	//th.addToReadyQ(1);

	// run exec, check ready, pull test
	th.run();
}
// ---------------------------------------
