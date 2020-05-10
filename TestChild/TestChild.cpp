#include "TestChild.h"

// TODO: 
// - output 
int main(int argc, char* argv[]) {

    // arguments to vars
    int processId = atoi(argv[0]);
    int inPort = atoi(argv[1]);
    int outPort = atoi(argv[2]);

    // init comms
    MsgPassingCommunication::EndPoint epPeer1("localhost", outPort);
    MsgPassingCommunication::EndPoint epPeer2("localhost", inPort);
    MsgPassingCommunication::Comm comm(epPeer2, "Peer2");
    comm.start();

    // send ready message
    std::cout << "Sending Ready message." << std::endl;
    MsgPassingCommunication::Message ready_msg;
    ready_msg.to(epPeer1);
    ready_msg.from(epPeer2);
    ready_msg.command("ready");
    std::string processIdStr = Utilities::Converter<size_t>::toString(processId);
    ready_msg.procid(processIdStr);
    comm.postMessage(ready_msg);

    // receive msg
    MsgPassingCommunication::Message msg;
    while (true)
    {
        msg = comm.getMessage();
        std::cout << "Received tester message \"" << msg.file() << "\"";
        if (msg.command() == "request")
        {
            // process message:
            // test dll
            // get results 
            // send results msg back
            // send ready message
            DllLoader dllLoader = DllLoader(msg.file());
            dllLoader.TestITest();
            TestResult testResult = dllLoader.getTestResult();
            std::cout << testResult.dll_path_ << std::endl;
            std::cout << testResult.test_result_ << std::endl;
            std::cout << testResult.log_message_ << std::endl;
        }
        if (msg.command() == "stop")
            break;
    }

    comm.stop();
}