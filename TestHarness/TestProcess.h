
#ifndef TESTPROCESS_H
#define TESTPROCESS_H
#ifndef WIN32_LEAN_AND_MEAN  // prevents duplicate includes of core parts of windows.h in winsock2.h
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <iostream>

#include "../MsgPassingComm/Comm.h"
#include "../Message/Message.h"

class TestProcess
{
public:

	TestProcess(int id, int portIn, int portOut);
	MsgPassingCommunication::EndPoint * getEndPoint() { return ep_; }

private:

	void initCommunication( int port_ );
	void createProcess( void );
	int processId_;
	int portIn_;
	int portOut_;
	MsgPassingCommunication::EndPoint* ep_;


};

#endif