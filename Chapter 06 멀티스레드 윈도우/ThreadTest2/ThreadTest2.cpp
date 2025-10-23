#include <windows.h>
#include <stdio.h>

DWORD WINAPI MyThread(LPVOID arg)
{
	while (1);
	return 0;
}

int main()
{
	printf("우선순위: %d ~ %d\n", THREAD_PRIORITY_IDLE,
		THREAD_PRIORITY_TIME_CRITICAL);

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	for (int i = 0; i < (int)si.dwNumberOfProcessors; i++) {
		HANDLE hThread = CreateThread(NULL, 0, MyThread, NULL, 0, NULL);
		SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		CloseHandle(hThread);
	}

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
	Sleep(1000);
	printf("주 스레드 실행!\n");
	return 0;
}