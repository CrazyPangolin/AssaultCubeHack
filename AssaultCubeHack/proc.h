#pragma once

#include<vector>
#include<Windows.h>
#include<TlHelp32.h>

DWORD GetProcId(const wchar_t* proc_name);

uintptr_t GetModuleBaseAddress(DWORD proc_id, const wchar_t* module_name);

uintptr_t FindDMAAddy(HANDLE h_process, uintptr_t ptr, std::vector<uintptr_t>);
