#include <windows.h>
#include <stdio.h>
#include <process.h>

struct Point3D
{
	int x, y, z;
};

unsigned __stdcall MyThread(LPVOID arg)
{
	Sleep(1000);
	Point3D* pt = (Point3D*)arg;
	printf("Running MyThread() %d: %d, %d, %d\n",
		GetCurrentThreadId(), pt->x, pt->y, pt->z);
	//_endthreadex(0);
	return 0;
}

int main(int argc, char *argv[])
{
	Point3D pt1 = { 10, 20, 30 };
	HANDLE hThread1 = (HANDLE)_beginthreadex(NULL, 0, MyThread, &pt1, 0, NULL);
	if (hThread1 == NULL) return 1;
	CloseHandle(hThread1);

	Point3D pt2 = { 40, 50, 60 };
	HANDLE hThread2 = (HANDLE)_beginthreadex(NULL, 0, MyThread, &pt2, 0, NULL);
	if (hThread2 == NULL) return 1;
	CloseHandle(hThread2);

	printf("Running main() %d\n", GetCurrentThreadId());
	Sleep(2000);
	return 0;
}