#include "stdafx.h"
#include "UMDHHelper.h"
#include <iostream>
#include <windows.h>


static const wchar_t *g_lpszUmdhPath = L"D:\\Software\\Debugging.Tools.for.Windows.x86\\UMDH.exe";


struct HandleHelper
{
	HandleHelper() : hHandle(INVALID_HANDLE_VALUE) {}
	HandleHelper(HANDLE h) : hHandle(h) {}
	~HandleHelper()
	{
		Release();
	}

	operator HANDLE () { return hHandle; }

	void Release()
	{
		if (hHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hHandle);
			hHandle = INVALID_HANDLE_VALUE;
		}
	}

	HANDLE hHandle;
};

static bool StartUmdh(const wchar_t *lpszCmdLine)
{
	STARTUPINFO stInfo;
	memset(&stInfo, 0, sizeof(STARTUPINFO));
	stInfo.cb = sizeof(STARTUPINFO);
	stInfo.dwFlags = STARTF_USESHOWWINDOW;
	stInfo.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION proInfo;
	memset(&proInfo, 0, sizeof(PROCESS_INFORMATION));
	if (!::CreateProcess(NULL, const_cast<wchar_t*>(lpszCmdLine), NULL, NULL, FALSE, 0, NULL, NULL, &stInfo, &proInfo))
        return false;

	CloseHandle(proInfo.hThread);
	HandleHelper hProcess(proInfo.hProcess);

	// 等待进程执行结束
	DWORD dwErr = WaitForSingleObject(hProcess, 10 * 60 * 1000);
	if (dwErr != WAIT_OBJECT_0)
	{
		TerminateProcess(hProcess, 1);
		return false;
	}

	return true;
}

bool umdh::CheckMemoryPointByUmdh(int iProcessId, const wchar_t *lpszSanpshotPath)
{
	wchar_t lpszProcessId[30] = { 0 };
	swprintf_s(lpszProcessId, L"%d", iProcessId);

	// umdh -p:1111 -f:log1.txt
	std::wstring strCmdLine;
	strCmdLine += L"\"";
	strCmdLine += g_lpszUmdhPath;
	strCmdLine += L"\" -p:";
	strCmdLine += lpszProcessId;
	strCmdLine += L" -f:\"";
	strCmdLine += lpszSanpshotPath;
	strCmdLine += L"\"";

	return StartUmdh(strCmdLine.c_str());
}

bool umdh::DiffByUmdh(const wchar_t *lpszSnapshotOld, const wchar_t *lpszSnapshotNew, const wchar_t *lpszResult)
{
	// umdh - d log1.txt log2.txt > result.txt
	std::wstring strCmdLine;
	strCmdLine += L"\"";
	strCmdLine += g_lpszUmdhPath;
	strCmdLine += L"\" -d \"";
	strCmdLine += lpszSnapshotOld;
	strCmdLine += L"\" \"";
	strCmdLine += lpszSnapshotNew;
	strCmdLine += L"\" -f:\"";
	strCmdLine += lpszResult;
	strCmdLine += L"\"";

	return StartUmdh(strCmdLine.c_str());
}

static bool StartUmdhEx(const wchar_t *lpszCmdLine, const wchar_t *lpszFilePath)
{
	// 创建并使用匿名管道进行进程间通信
	HandleHelper hRead;
	HandleHelper hWrite;
	SECURITY_ATTRIBUTES sa;
	memset(&sa, 0, sizeof(SECURITY_ATTRIBUTES));
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	if (!::CreatePipe(&(hRead.hHandle), &(hWrite.hHandle), &sa, 1 * 1024 * 1024))
		return false;

	STARTUPINFO stInfo;
	memset(&stInfo, 0, sizeof(STARTUPINFO));
	stInfo.cb = sizeof(STARTUPINFO);
	stInfo.wShowWindow = SW_HIDE;
	stInfo.hStdOutput = hWrite;
	stInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	PROCESS_INFORMATION proInfo;
	memset(&proInfo, 0, sizeof(PROCESS_INFORMATION));
	if (!::CreateProcess(NULL, const_cast<wchar_t*>(lpszCmdLine), NULL, NULL, TRUE, 0, NULL, NULL, &stInfo, &proInfo))
		return false;

	hWrite.Release();
	CloseHandle(proInfo.hProcess);
	CloseHandle(proInfo.hThread);

	HandleHelper hResult(CreateFile(lpszFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL));
	if (hResult == INVALID_HANDLE_VALUE)
		return false;

	const int iBufSize = 1024;
	char buf[iBufSize] = { 0 };
	DWORD dwReadSize = 0;
	DWORD dwWriteSize = 0;
	while (true)
	{
		if (ReadFile(hRead, buf, iBufSize, &dwReadSize, NULL) && dwReadSize > 0)
			WriteFile(hResult, buf, dwReadSize, &dwWriteSize, NULL);
		else
			break;
	}

	return true;
}

bool umdh::CheckMemoryPointByUmdhEx(int iProcessId, const wchar_t *lpszSanpshotPath)
{
	wchar_t lpszProcessId[30] = { 0 };
	swprintf_s(lpszProcessId, L"%d", iProcessId);

	// umdh -p:1111
	std::wstring strCmdLine;
	strCmdLine += L"\"";
	strCmdLine += g_lpszUmdhPath;
	strCmdLine += L"\" -p:";
	strCmdLine += lpszProcessId;

	return StartUmdhEx(strCmdLine.c_str(), lpszSanpshotPath);
}

bool umdh::DiffByUmdhEx(const wchar_t *lpszSnapshotOld, const wchar_t *lpszSnapshotNew, const wchar_t *lpszResult)
{
	// umdh - d log1.txt log2.txt
	std::wstring strCmdLine;
	strCmdLine += L"\"";
	strCmdLine += g_lpszUmdhPath;
	strCmdLine += L"\" -d \"";
	strCmdLine += lpszSnapshotOld;
	strCmdLine += L"\" \"";
	strCmdLine += lpszSnapshotNew;
	strCmdLine += L"\"";

	return StartUmdhEx(strCmdLine.c_str(), lpszResult);
}

