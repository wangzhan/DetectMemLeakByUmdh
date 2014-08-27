// Author: WangZhan -> wangzhan.1985@gmail.com
#pragma once


// ׼������:
// 1. ��װ windbg ���Թ��ߣ������ umdh �� gflags
// 2. ���÷��ű�Ļ������� _NT_SYMBOL_PATH = SRV*c:\symbols*http://msdl.microsoft.com/download/symbols;c:\myapplicationssymbols
// 3. ʹ�� gflags ��ָ����ִ���ļ��Ķ�ջ���ٱ�ʶ gflags -i appname.exe +ust
// 4. ʹ�ý���ģʽ����־ģʽ�ֱ�������̶�ջ�Ŀ���
// 4.1 umdh -p:1111 -f:log1.txt
// 4.2 umdh -p:1111 -f:log2.txt
// 4.3 umdh -d log1.txt log2.txt > result.txt

namespace umdh
{
	// ����ָ�����̿��գ��ȴ� umdh ���̽���
	bool CheckMemoryPointByUmdh(int iProcessId, const wchar_t *lpszSanpshotPath);
	
	// �ȽϿ����ļ�
	bool DiffByUmdh(const wchar_t *lpszSnapshotOld, const wchar_t *lpszSnapshotNew, const wchar_t *lpszResult);

	// ����ָ�����̿��գ�ʹ�������ܵ��㶨���̼�ͨ��
	bool CheckMemoryPointByUmdhEx(int iProcessId, const wchar_t *lpszSanpshotPath);
	
	// ʹ�������ܵ�
	bool DiffByUmdhEx(const wchar_t *lpszSnapshotOld, const wchar_t *lpszSnapshotNew, const wchar_t *lpszResult);
}

