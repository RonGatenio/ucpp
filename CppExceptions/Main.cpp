#include <Windows.h>
#include <intrin.h>
#include <stdio.h>
#include <exception>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <evntrace.h>


extern "C" {
	extern void** __imp_RaiseException;

	void __stdcall MyRaiseException(
		DWORD dwExceptionCode,
		DWORD dwExceptionFlags,
		DWORD nNumberOfArguments,
		const ULONG_PTR* lpArguments);


	extern void** __imp_RtlVirtualUnwind;

	PEXCEPTION_ROUTINE
		MyRtlVirtualUnwind(
			IN ULONG HandlerType,
			IN ULONG64 ImageBase,
			IN ULONG64 ControlPc,
			IN PRUNTIME_FUNCTION FunctionEntry,
			IN OUT PCONTEXT ContextRecord,
			OUT PVOID* HandlerData,
			OUT PULONG64 EstablisherFrame,
			IN OUT PKNONVOLATILE_CONTEXT_POINTERS ContextPointers OPTIONAL
		);

	extern void** __imp_RtlUnwindEx;

	VOID
		MyRtlUnwindEx(
			IN PVOID TargetFrame OPTIONAL,
			IN PVOID TargetIp OPTIONAL,
			IN PEXCEPTION_RECORD ExceptionRecord OPTIONAL,
			IN PVOID ReturnValue,
			IN PCONTEXT OriginalContext,
			IN PUNWIND_HISTORY_TABLE HistoryTable OPTIONAL
		);

	extern void** __imp_RtlUnwind;
	// XXX
	VOID
		MyRtlUnwind(
			IN PVOID TargetFrame OPTIONAL,
			IN PVOID TargetIp OPTIONAL,
			IN PEXCEPTION_RECORD ExceptionRecord OPTIONAL,
			IN PVOID ReturnValue
		);


}

//
//extern "C"
//{
//	void* readsspq();
//	void* readsspd();
//}
//
//void* readssp()
//{
//#ifdef _WIN64
//	return readsspq();
//#else
//	return readsspd();
//#endif // _WIN64
//
//}

void* ll()
{
	std::ofstream MyFile("filename.txt");

	DWORD o;
	VirtualProtect(&__imp_RaiseException, 8, PAGE_READWRITE, &o);
	*(void**)&__imp_RaiseException = (void*)MyRaiseException;

	VirtualProtect(&__imp_RtlVirtualUnwind, 8, PAGE_READWRITE, &o);
	*(void**)&__imp_RtlVirtualUnwind = (void*)MyRtlVirtualUnwind;

	VirtualProtect(&__imp_RtlUnwindEx, 8, PAGE_READWRITE, &o);
	*(void**)&__imp_RtlUnwindEx = (void*)MyRtlUnwindEx;

	VirtualProtect(&__imp_RtlUnwind, 8, PAGE_READWRITE, &o);
	*(void**)&__imp_RtlUnwind = (void*)MyRtlUnwind;
	
	throw std::exception();
	
	MyFile.close();
}

int main(int argc, char** args)
{
	std::vector<uint8_t> a{ 1, 2, 3 };
	ProcessTrace(NULL, 0, NULL, NULL);
	try
	{
		try
		{
			ll();
		}
		catch (const int&)
		{
			printf("exception 1..\n");
		}
		catch (...)
		{
			printf("exception 2..\n");
			// TODO: fix throw;
		}
	}
	catch (const std::exception&)
	{
		printf("exception 3..\n");
	}
	catch (...)
	{
		printf("exception 4..\n");
	}
	
    return 0;
}