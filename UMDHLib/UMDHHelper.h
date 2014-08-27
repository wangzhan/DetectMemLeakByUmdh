// Author: WangZhan -> wangzhan.1985@gmail.com
#pragma once


// 准备工作:
// 1. 安装 windbg 调试工具，其包含 umdh 和 gflags
// 2. 设置符号表的环境变量 _NT_SYMBOL_PATH = SRV*c:\symbols*http://msdl.microsoft.com/download/symbols;c:\myapplicationssymbols
// 3. 使用 gflags 打开指定可执行文件的堆栈跟踪标识 gflags -i appname.exe +ust
// 4. 使用进程模式和日志模式分别分析进程堆栈的快照
// 4.1 umdh -p:1111 -f:log1.txt
// 4.2 umdh -p:1111 -f:log2.txt
// 4.3 umdh -d log1.txt log2.txt > result.txt

namespace umdh
{
	// 捕获指定进程快照，等待 umdh 进程结束
	bool CheckMemoryPointByUmdh(int iProcessId, const wchar_t *lpszSanpshotPath);
	
	// 比较快照文件
	bool DiffByUmdh(const wchar_t *lpszSnapshotOld, const wchar_t *lpszSnapshotNew, const wchar_t *lpszResult);

	// 捕获指定进程快照，使用匿名管道搞定进程间通信
	bool CheckMemoryPointByUmdhEx(int iProcessId, const wchar_t *lpszSanpshotPath);
	
	// 使用匿名管道
	bool DiffByUmdhEx(const wchar_t *lpszSnapshotOld, const wchar_t *lpszSnapshotNew, const wchar_t *lpszResult);
}

