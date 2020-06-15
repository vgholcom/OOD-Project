/*
 *-------------------------------------
 * Yuexin Yu
 * CSE 687 Object Oriented Design C++
 * Syracuse University
 * Design Project: Test Harness
 *
 * Victoria Tenney
 *-------------------------------------
 * Defined the TestHarness class for
 * TestHarness package.
 * ----------
 */

#ifndef TESTHARNESS_H
#define TESTHARNESS_H

#include "Process.h"
#include "../SupportLibrary/MsgPassingComm/CommHeader.h"
#include "../SupportLibrary/Sockets/Sockets.h"
#include "../SupportLibrary/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../SupportLibrary/Message/Message.h"
#include "../SupportLibrary/Utilities/XmlReader.h"
#include "../ChildTester/Child.h"
#include <vector>

using namespace MsgPassingCommunication;
namespace Testing
{
	class TestHarnessServer
	{
	public:
		TestHarnessServer(EndPoint from);
		~TestHarnessServer() = default;
		void start();
		void wait();
		void stop();
		void sendMsg(MsgPassingCommunication::Message msg);
		void recvMessages();
		void dispatchMessages();
		void setPath(std::string& send_path, std::string& save_path);
		std::string getSendPath();
		std::string getSavePath();
	private:
		BlockingQueue<MsgPassingCommunication::Message> readyQ_;
		BlockingQueue<MsgPassingCommunication::Message> requestQ_;
		std::thread recvr;
		std::thread dspat;
		Comm comm_;
		EndPoint to_;
		EndPoint from_;
		std::vector<Process> children_;
	};
}
#endif
