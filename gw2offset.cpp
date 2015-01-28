#include "stdafx.h"
#include <iostream>
#include <fstream>

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	HWND hwnd;
	hwnd = FindWindow(NULL, L"Guild Wars 2");
	if (!hwnd) { cout << "Window not found!\n"; cin.get(); }
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE phandle = OpenProcess(PROCESS_VM_READ, 0, pid);
	if (!phandle) { cout << "Could not get handle!\n"; cin.get(); }

	ofstream file;
	file.open("out.txt");
	file << hex;
	file.close();
	cout << hex;
	cout << "Loop Start" << endl;

	unsigned long base = 0x2f22e2b0; // agPtr, not *agPtr

	if (0) // scan 4 offsets deep (ie. world boss hp)
	{
		for (unsigned long offs1 = 0x30; offs1 < 0x34; offs1 += 0x4)
		{
			for (unsigned long offs2 = 0x0; offs2 < 0x300; offs2 += 0x4)
			{
				for (unsigned long offs3 = 0x0; offs3 < 0x300; offs3 += 0x4)
				{
					for (unsigned long offs4 = 0x0; offs4 < 0x300; offs4 += 0x4)
					{
						unsigned long addr = base;
						ReadProcessMemory(phandle, (void*)addr, &addr, sizeof(addr), 0);
						ReadProcessMemory(phandle, (void*)(addr + offs1), &addr, sizeof(addr), 0);
						ReadProcessMemory(phandle, (void*)(addr + offs2), &addr, sizeof(addr), 0);
						ReadProcessMemory(phandle, (void*)(addr + offs3), &addr, sizeof(addr), 0);
						ReadProcessMemory(phandle, (void*)(addr + offs4), &addr, sizeof(addr), 0);

						float cHealth = 0; float mHealth = 0;
						ReadProcessMemory(phandle, (void*)(addr + 0x8), &cHealth, sizeof(cHealth), 0);
						ReadProcessMemory(phandle, (void*)(addr + 0xc), &mHealth, sizeof(mHealth), 0);

						// NaN
						if (cHealth != cHealth || mHealth != mHealth)
							continue;

						// too low
						if (cHealth < 10000)
							continue;
						if (mHealth < 10000)
							continue;

						// too high
						if (mHealth > 30000000)
							continue;

						// invalid
						if (cHealth > mHealth)
							continue;
						if (mHealth != floor(mHealth))
							continue;

						// full health
						if (cHealth == mHealth)
							continue;

						cout << "[" << offs1 << "][" << offs2 << "][" << offs3 << "][" << offs4 << "] " << mHealth << endl;
						file.open("out.txt", ios::app);
						file << "[" << offs1 << "][" << offs2 << "][" << offs3 << "][" << offs4 << "] " << mHealth << " -> " << cHealth << endl;
						file.close();
					}
				}
			}
		}
	}
	
	if (1) // scan 3 offsets deep (ie. char lvl)
	{
		for (unsigned long offs1 = 0x30; offs1 < 0x34; offs1 += 0x4) // (0x30)
		{
			for (unsigned long offs2 = 0x0; offs2 < 0x300; offs2 += 0x4)
			{
				for (unsigned long offs3 = 0x0; offs3 < 0x300; offs3 += 0x2)
				{
					unsigned long addr = base;
					ReadProcessMemory(phandle, (void*)addr, &addr, sizeof(addr), 0);
					ReadProcessMemory(phandle, (void*)(addr + offs1), &addr, sizeof(addr), 0);
					ReadProcessMemory(phandle, (void*)(addr + offs2), &addr, sizeof(addr), 0);
					
					short int value = 0;
					ReadProcessMemory(phandle, (void*)(addr + offs3), &value, sizeof(value), 0);

					// expected value
					if (value != 2313)
						continue;

					cout << "[" << offs1 << "][" << offs2 << "][" << offs3 << "] Value: " << value << endl;
					file.open("out.txt", ios::app);
					file << "[" << offs1 << "][" << offs2 << "][" << offs3 << "]" << endl;
					file.close();
				}
			}
		}
	}

	cout << "Loop End" << endl;
	system("pause>nul");
	return 0;
}
