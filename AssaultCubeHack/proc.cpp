#include "proc.h"
DWORD GetProcId(const wchar_t* proc_name)
{

	DWORD process_id = 0;
	HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(h_snap!= INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 process_entry;
		process_entry.dwSize = sizeof(PROCESSENTRY32);
		if(Process32First(h_snap, &process_entry))
		{
			do
			{
				if (!_wcsicmp(process_entry.szExeFile, proc_name))
				{
					process_id = process_entry.th32ProcessID;
					break;
				}
			} while (Process32Next(h_snap, &process_entry));
		}
	}
	CloseHandle(h_snap);
	return process_id;
}

uintptr_t GetModuleBaseAddress(DWORD proc_id, const wchar_t* module_name)
{
	uintptr_t module_base_address = 0;
	HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id);
	if (h_snap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 module_entry;
		module_entry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(h_snap, &module_entry))
		{
			do
			{
				if (!_wcsicmp(module_entry.szModule, module_name))
				{
					module_base_address = (uintptr_t)module_entry.modBaseAddr;
					break;
				}
			} while (Module32Next(h_snap, &module_entry));
		}
	}
	CloseHandle(h_snap);
	return module_base_address;
}

uintptr_t FindDMAAddy(HANDLE h_process, uintptr_t ptr, std::vector<uintptr_t> offsets)
{
	uintptr_t addr = ptr;
	for(size_t i =0; i< offsets.size(); i++)
	{
		ReadProcessMemory(h_process, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}