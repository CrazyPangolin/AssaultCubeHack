
#include<Windows.h>
#include<vector>
#include<iostream>
#include"proc.h"


//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//                     _In_opt_ HINSTANCE hPrevInstance,
//                     _In_ LPWSTR    lpCmdLine,
//                     _In_ int       nCmdShow)
//{
//   
int main()
{
    //Get ProcId of the target process
	DWORD process_id = GetProcId(L"ac_client.exe");

	//GetModuleBaseAddress
	uintptr_t module_base_address = GetModuleBaseAddress(process_id, L"ac_client.exe");

	// Resolve base address of the pointer chain

	HANDLE h_process = NULL;
	h_process = OpenProcess(PROCESS_ALL_ACCESS,NULL, process_id);
	uintptr_t dynamic_ptr_base_addr = module_base_address + 0x109B74;

	std::cout << "DynamicPtrAddress = " << "0x" << std::hex << dynamic_ptr_base_addr << std::endl;
	
	//Resolve our ammo pointer chain

	std::vector<uintptr_t> ammo_offsets = { 0x374, 0x14, 0 };
	uintptr_t ammo_address = FindDMAAddy(h_process, dynamic_ptr_base_addr, ammo_offsets);
	std::cout << "ammo_address = " << "0x" << std::hex << ammo_address << std::endl;
	//Read Ammo value
	int ammo_value = 0;
	ReadProcessMemory(h_process, (BYTE*)ammo_address, &ammo_value, sizeof(ammo_value), nullptr);
	std::cout << "ammo_value = " << std::dec << ammo_value << std::endl;
	// Write to it
	int new_ammo_value = 999;
	WriteProcessMemory(h_process, (BYTE*)ammo_address, &new_ammo_value, sizeof(new_ammo_value), nullptr);
	
	// Read out again
	int new_ammo_value_read_again = 0;
	ReadProcessMemory(h_process, (BYTE*)ammo_address, &new_ammo_value_read_again, sizeof(new_ammo_value_read_again), nullptr);
	std::cout << "new_ammo_value = " << std::dec << new_ammo_value_read_again << std::endl;

	CloseHandle(h_process);
    return 0;
}

