/*
*  Package Operations:
*  -------------------
*  This package provides a class, Process, used to start named processes.
*  It has a lot of potential, mostly unrealized by this simple beginning.
*
*  Possible future features include:
*  - interprocess communication between parent and child using pipes
*  - handling multiple process, perhaps in a process pool
*  - killing processes
*  - enumerating running processes that match a regular expression
*/

#ifndef WIN32_LEAN_AND_MEAN  // prevents duplicate includes of core parts of windows.h in winsock2.h
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <string>
#include <iostream>
#include <functional>

using CBP = std::function<void(void)>;

class Process
{
public:
	Process();
	virtual ~Process() {}
	bool create(const std::string& path = "");
	void title(const std::string& title);
	void application(const std::string& app);
	void commandLine(const std::string& cmdLine);
	void registerCallback();
	void setCallBackProcessing(CBP cbp);
	void callback();
private:
	STARTUPINFO startInfo_;
	PROCESS_INFORMATION procInfo_;
	std::wstring title_;
	std::wstring appName_;
	std::wstring commandLine_;
	static CBP cbp_;
};

CBP Process::cbp_ = []() { std::cout << "\n\n-- Child Process Exited --"; };

// Process Constructor 
/*
* - This eventually will have more functionality
*/
inline Process::Process()
{
	GetStartupInfo(&startInfo_);
	startInfo_.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
	startInfo_.dwX = 200;
	startInfo_.dwY = 250;
	startInfo_.dwYSize = 300;
}

// Helper function to convert strings
std::string wToS(const std::wstring& ws)
{
	std::string conv;
	for (auto ch : ws)
	{
		conv.push_back(static_cast<char>(ch));
	}
	return conv;
}

// Helper function to convert strings 
std::wstring sToW(const std::string& s)
{
	std::wstring conv;
	for (auto ch : s)
	{
		conv.push_back(static_cast<wchar_t>(ch));
	}
	return conv;
}

// Define application new process runs 
inline void Process::application(const std::string& appName)
{
	appName_ = sToW(appName);
}

// Define commandline passed to new application 
inline void Process::commandLine(const std::string& cmdLine)
{
	commandLine_ = sToW(cmdLine);
}

// Define title bar for console window 
/*
* - applications like Notepad overwrite this string
*/
inline void Process::title(const std::string& title)
{
	title_ = sToW(title);
	startInfo_.lpTitle = const_cast<LPWSTR>(title_.c_str());
}

// Start new child process 
inline bool Process::create(const std::string& appName)
{
	std::wstring app;
	if (appName.size() == 0)
		app = appName_;
	else
	{
		app = sToW(appName);
	}
	LPCTSTR applic = app.c_str();
	LPTSTR cmdLine = const_cast<LPTSTR>(commandLine_.c_str());
	LPSECURITY_ATTRIBUTES prosec = nullptr;
	LPSECURITY_ATTRIBUTES thrdsec = nullptr;
	BOOL inheritHandles = false;
	// or CREATE_NO_WINDOW
	DWORD createFlags = CREATE_NEW_CONSOLE;  
	LPVOID environment = nullptr;
	LPCTSTR currentPath = nullptr;
	LPSTARTUPINFO pStartInfo = &startInfo_;
	LPPROCESS_INFORMATION pPrInfo = &procInfo_;

	BOOL OK =
		CreateProcess(
			applic, cmdLine, prosec, thrdsec, inheritHandles,
			createFlags, environment, currentPath, pStartInfo, pPrInfo
		);
	return OK;
}


// Child Process Exit Callback Processing

// Provide new callable object for callback to invoke 
void Process::setCallBackProcessing(CBP cbp)
{
	cbp_ = cbp;
}

// Function called when child process exists 
void Process::callback()
{
	cbp_();
}

// Windows API declared function type for callbacks 
void CALLBACK WaitOrTimerCallback(_In_ PVOID lpParameter, _In_ BOOLEAN TimerOrWaitFired)
{
	Process p;
	p.callback();
	return;
}

// Register callback with Windows API 
void Process::registerCallback()
{
	HANDLE hNewHandle;
	HANDLE hProcHandle = procInfo_.hProcess;
	RegisterWaitForSingleObject(&hNewHandle, hProcHandle, WaitOrTimerCallback, NULL, INFINITE, WT_EXECUTEONLYONCE);
}
