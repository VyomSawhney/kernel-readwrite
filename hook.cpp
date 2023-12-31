
#include "hook.h"

bool nullhook::call_kernal_function(void* kernel_function_address)
{
    if (!kernel_function_address)
        return false;

    PVOID* function = reinterpret_cast<PVOID*>(get_system_module_export("\\Systemroot\\System32\\drivers\\dxgkrnl.sys", "NtQueryCompositionSurfaceStatistics"));
    
    if (!function)
    {
        DbgPrint("Failed to get the address of NtQueryCompositionSurfaceStatistics.");
        return false;
    }

    BYTE orig[] = { 0x00, 0x00, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };

    BYTE shell_code[] = { 0x48, 0xB8 }; // push xxx; pop rax
    BYTE shell_code_end[] = { 0xFF, 0xE0 }; //jmp rax

    RtlSecureZeroMemory(&orig, sizeof(orig));
    memcpy((PVOID)((ULONG_PTR)orig), &shell_code, sizeof(shell_code));
    uintptr_t hook_address = reinterpret_cast<uintptr_t>(kernel_function_address);
    memcpy((PVOID)((ULONG_PTR)orig + sizeof(shell_code)), &hook_address, sizeof(void*));
    memcpy((PVOID)((ULONG_PTR)orig + sizeof(shell_code) + sizeof(void*)), &shell_code_end, sizeof(shell_code_end));

    if (!write_to_read_only_memory(function, &orig, sizeof(orig)))
    {
        DbgPrint("Failed to write shellcode to target function.");
        return false;
    }

    DbgPrint("Hooking successful.");
    return true;
}

NTSTATUS nullhook::hook_handler(PVOID called_param)
{
    return STATUS_SUCCESS;
}
