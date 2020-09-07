/* DriverVodka Author C4ke#0002
*  Ты думаешь это магазин с водярой? 
*  Я скажу так, ди нахер
*/
#pragma once
#include <ntifs.h>
#include <ntddk.h>
#include <windef.h>

#define Print(a, ...) DbgPrintEx(0, 0, a, __VA_ARGS__);

int PIDVodka;

const uintptr_t offset_typeReq = 0x3058;//0x3058
const uintptr_t offset_memAddress = 0x3048;//0x3048
const uintptr_t offset_memSize = 0x3050;//0x3050
const uintptr_t offset_retAnswer = 0x3038;//0x3038
const uintptr_t offset_fRetAnswer = 0x3040;//0x3040
const uintptr_t offset_pid = 0x3060;//0x3060

#pragma region NTKERNELAPI
extern "C" {
	NTKERNELAPI
		PVOID
		PsGetProcessSectionBaseAddress(
			__in PEPROCESS Process
		);

	NTKERNELAPI
		NTSTATUS
		NTAPI
		MmCopyVirtualMemory(PEPROCESS SourceProcess, PVOID SourceAddress, PEPROCESS TargetProcess, PVOID TargetAddress, SIZE_T BufferSize, KPROCESSOR_MODE PreviousMode, PSIZE_T ReturnSize);
}
#pragma endregion

#pragma region Methods
uintptr_t ReadProcessMemory(uintptr_t, uintptr_t, uintptr_t);
uintptr_t WriteProcessMemory(uintptr_t, uintptr_t, uintptr_t, uintptr_t);
uintptr_t GetBaseAddress(int);
void MainHandler(void*);
#pragma endregion
