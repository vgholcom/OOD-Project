///////////////////////////////////////////////////////////////////////
// TestHarness.h                                                     //
// ver 1.0                                                           //
// Victoria Tenney, CSE687 - Object Oriented Design, Spring 2018     //
///////////////////////////////////////////////////////////////////////
// Requirements:
// - accept test request xml string defining one or more test elements 
// - create, at startup, a number of child tester processes
// - child tester processes should post ready messages to the test harness
// - process test request message and send test status message
// - on receiving test request, test harness shall parse the request in to 
//   individual tests and assign each test to a single child tester
 

#ifndef TESTHARNESS_H
#define TESTHARNESS_H

#include "..\Cpp11-BlockingQueue\Cpp11-BlockingQueue.h"
#include "../MsgPassingComm/Comm.h"
#include "../Message/Message.h"
#include "TestProcess.h"

#include <vector>
#include <iostream>
#include <thread>
#include <functional>

class TestHarness
{
public:

	TestHarness(void);

	void run(void);

	// TODO: move to private when operational 
	//       public for class testing
	void addToTestRequestQ(std::vector<std::string> dlls);
	void addToReadyQ(uint8_t pid);

private:

	void createChildProcesses(int N);

	// a blocking queue for test request
	BlockingQueue<std::string> testRequestQ_;

	// a blocking queue for ready messages
	BlockingQueue<uint8_t> readyMsgQ_;

	// child thread to extract messages from its comm
	// & enqueue the message in the appropriate queue
	void recvMessage(  ); //MsgPassingCommunication::Comm& comm

	// main test harness thread dequeues these and 
	// sends the test request to the child tester
	// that sent the ready messsage
	void testHarnessExec( void );

	// comms
	int hostPort_ = 8081;
	MsgPassingCommunication::Comm* comm;
	MsgPassingCommunication::EndPoint* epPeer1;
	MsgPassingCommunication::EndPoint* epPeer2;
	MsgPassingCommunication::EndPoint* epPeer3;

};

#endif