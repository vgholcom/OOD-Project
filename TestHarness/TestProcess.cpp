#include "TestProcess.h"

TestProcess::TestProcess(int id, int portIn, int portOut)
    : processId_( id )
    , portIn_( portIn )
    , portOut_( portOut )
{
    initCommunication(portIn_);
    createProcess();
}

void TestProcess::initCommunication(int portIn_)
{
    ep_ = new MsgPassingCommunication::EndPoint("localhost", portIn_);
}



// TODO: 
// properly terminate
// ready variable ? 
void TestProcess::createProcess( void )
{
    char* applicationName;
    applicationName = const_cast <char*>("..\\Debug\\TestChild.exe");

    char pCL[25];
    sprintf_s(pCL, "%d %d %d", processId_, portIn_, portOut_);

    // arguments for create process:
    STARTUPINFO startInfo;                      // establishes properties of child process
    PROCESS_INFORMATION procInfo;               // holds info returned from CreateProcess
    LPCTSTR appName = applicationName;          // program to run
    LPTSTR  commandLine = (pCL);                // main's args
    LPSECURITY_ATTRIBUTES procSec = NULL;       // default security
    LPSECURITY_ATTRIBUTES thrdSec = NULL;       // default security
    BOOL inheritHandles = FALSE;                // don't inherit hndls
    DWORD createFlags = CREATE_NEW_CONSOLE;     // make a console
    LPVOID environment = NULL;                  // caller's evironmt
    LPCTSTR currentPath = NULL;                 // default dir
    LPSTARTUPINFO pStartInfo = &startInfo;      // set process style
    LPPROCESS_INFORMATION pPrInfo = &procInfo;  // retrieve info

    std::string buildTitle = std::string("Child process: ") += *applicationName;
    GetStartupInfo(&startInfo);
    startInfo.lpTitle = (char*)buildTitle.c_str();
    startInfo.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
    startInfo.dwX = 200;
    startInfo.dwY = 250;
    startInfo.dwXSize = 600;
    startInfo.dwYSize = 300;

    BOOL OK =
    CreateProcess(
        appName, commandLine, procSec, thrdSec, inheritHandles,
        createFlags, environment, currentPath, pStartInfo, pPrInfo
    );

    CloseHandle(procInfo.hThread);  // parent needs no access to child thread       // TODO... create pipe?

    if (!OK) {
        std::cout << "\n failed to create process: " << appName << "\n\n";

        // ask system what type of error occurred
        DWORD errorCode = GetLastError();

        // map errorCode into a system defined error string
        DWORD dwFlags =
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER;
        LPCVOID lpSource = NULL;
        DWORD dwMessageID = errorCode;
        DWORD dwLanguageId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
        LPSTR lpBuffer;
        DWORD nSize = 0;
        va_list* Arguments = NULL;

        FormatMessage(
            dwFlags, lpSource, dwMessageID, dwLanguageId,
            (LPTSTR)&lpBuffer, nSize, Arguments
        );

        std::cout << "\n  error: ";
        if (lpBuffer)
            std::cout << lpBuffer << "\n  --- did you forget extension .exe?" << "\n\n";
        return;
    }

    std::cout << "\n\n";

    // wait for child process to terminate before terminating
    //WaitForSingleObject(procInfo.hProcess, INFINITE);
    //CloseHandle(procInfo.hProcess);

    return;
}