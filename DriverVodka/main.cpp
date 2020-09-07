/* DriverVodka Author C4ke#0002
*  Ты думаешь это магазин с водярой? 
*  Я скажу так, ди нахер
*/
#include "main.h"

extern "C" NTSTATUS DriverEntry(
	PDRIVER_OBJECT  driver_object,
	PUNICODE_STRING registry_path,
	UINT64 PID
)
{
	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(registry_path);
	PIDVodka = PID;
	HANDLE thread_handle = nullptr;
	PsCreateSystemThread(
		&thread_handle,
		GENERIC_ALL,
		nullptr,
		nullptr,
		nullptr,
		MainHandler,
		nullptr
	);

	return STATUS_SUCCESS;
}

void MainHandler(void*)
{
	Print("[VODKA PROJECT] Hello baby :D!\n");
	PEPROCESS getCProc = nullptr;
	uintptr_t bAddress = GetBaseAddress(PIDVodka);
	WriteProcessMemory(PIDVodka, bAddress + offset_typeReq, 0, 4);
	while (true)
	{
		int req = ReadProcessMemory(PIDVodka, bAddress + offset_typeReq, 4);
		if (req == 0)
			continue;
		else if (req == 1)
		{
			WriteProcessMemory(PIDVodka, bAddress + offset_typeReq, 0, 4);
			Print("[VODKA PROJECT] get base address...\n");
			int pid = ReadProcessMemory(PIDVodka, bAddress + offset_pid, 4);
			uintptr_t _bAddress = GetBaseAddress(pid);
			WriteProcessMemory(PIDVodka, bAddress + offset_retAnswer, _bAddress, sizeof(uintptr_t));
			Print("[VODKA PROJECT] Baseaddress (%X), PID (%i)\n", bAddress, pid);
		}
		else if (req == 2)
		{
			WriteProcessMemory(PIDVodka, bAddress + offset_typeReq, 0, 4);
			Print("[VODKA PROJECT] read address...\n");
			int pid = ReadProcessMemory(PIDVodka, bAddress + offset_pid, 4);
			int size = ReadProcessMemory(PIDVodka, bAddress + offset_memSize, 4);
			uintptr_t memAddress = ReadProcessMemory(PIDVodka, bAddress + offset_memAddress, 8);
			uintptr_t answer = ReadProcessMemory(pid, memAddress, size);
			WriteProcessMemory(PIDVodka, bAddress + offset_retAnswer, answer, size);
			Print("[VODKA PROJECT] PID (%i), Size (%i), memAddress (%X), answer (%X)\n", pid, size, memAddress, answer);
		}
		else if (req == 3)
		{
			WriteProcessMemory(PIDVodka, bAddress + offset_typeReq, 0, 4);
			Print("[VODKA PROJECT] read address (float)...\n");
			int pid = ReadProcessMemory(PIDVodka, bAddress + offset_pid, 4);
			uintptr_t memAddress = ReadProcessMemory(PIDVodka, bAddress + offset_memAddress, 8);
			uintptr_t answer = ReadProcessMemory(pid, memAddress, sizeof(float));
			WriteProcessMemory(PIDVodka, bAddress + offset_fRetAnswer, answer, sizeof(float));
			Print("[VODKA PROJECT] PID (%i), memAddress (%X), answer (%X)\n", pid, memAddress, answer);
		}
	}
}

uintptr_t WriteProcessMemory(uintptr_t PID, uintptr_t TargetAddress, uintptr_t WriteValue, uintptr_t Size)
{
	PEPROCESS SourceProcess;
	if (!NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)PID, &SourceProcess)))
		return 0;

	PEPROCESS TargetProcess = PsGetCurrentProcess();

	SIZE_T Result;

	__try {

		NTSTATUS status = MmCopyVirtualMemory(TargetProcess, &WriteValue, SourceProcess, (PVOID)TargetAddress, Size, KernelMode, &Result);
		ObDereferenceObject(SourceProcess);
		if (NT_SUCCESS(status))
			return 1;
		else
			return 0;
	}
	__except (MEMORY_MANAGEMENT) {
		return 0;
	}
}

uintptr_t GetBaseAddress(int pid)
{
	PEPROCESS process = nullptr;
	NTSTATUS  status = PsLookupProcessByProcessId(HANDLE(pid), &process);

	if (!NT_SUCCESS(status))
		return 0;

	const auto base_address = uintptr_t(PsGetProcessSectionBaseAddress(process));
	ObDereferenceObject(process);

	return base_address;
}


uintptr_t ReadProcessMemory(uintptr_t PID, uintptr_t SourceAddress, uintptr_t Size)
{
	if (!PID || !SourceAddress || !Size)
		return 0;

	PEPROCESS SourceProcess;
	if (!NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)PID, &SourceProcess)))
		return 0;

	PEPROCESS TargetProcess = PsGetCurrentProcess();
	uintptr_t readValue;
	SIZE_T Result;
	if (!TargetProcess || !SourceProcess)
		return 0;
	NTSTATUS status = MmCopyVirtualMemory(SourceProcess, (PVOID)SourceAddress, TargetProcess, &readValue, Size, KernelMode, &Result);
	ObDereferenceObject(SourceProcess);
	if (NT_SUCCESS(status))
		return readValue;
	else
		return 0;
}