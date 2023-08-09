#pragma once
#include "memory.h"

namespace nullhook
{
	bool call_kernal_function(void* kernal_function_address);
	NTSTATUS hook_handler(PVOID called_param);
}