#include <wdm.h>
#include <cstddef>

#include <vector>

void* __cdecl operator new(size_t Size, POOL_TYPE PoolType, ULONG PoolTag) noexcept(false)
{
    if (Size == 0)
    {
        Size = 1;
    }

#pragma warning(push)
#pragma warning(disable : 4996) // FIXME - deprecated function
    void* memory = ExAllocatePoolWithTag(PoolType, Size, PoolTag);
#pragma warning(pop)
    if (!memory)
    {
        //throw std::bad_alloc();
    }
    return memory;
}

void __cdecl operator delete(void* Memory, POOL_TYPE, ULONG PoolTag) noexcept
{
    if (Memory)
    {
        ExFreePoolWithTag(Memory, PoolTag);
    }
}

void __cdecl operator delete(void* Memory) noexcept
{
    if (Memory)
    {
        ExFreePool(Memory);
    }
}

void __cdecl operator delete(void* Memory, unsigned __int64) noexcept
{
    if (Memory)
    {
        ExFreePool(Memory);
    }
}

void* __cdecl operator new[](size_t Size, POOL_TYPE PoolType, ULONG PoolTag) noexcept(false)
{
    return operator new(Size, PoolType, PoolTag);
}

void __cdecl operator delete[](void* Memory, POOL_TYPE PoolType, ULONG PoolTag) noexcept
{
    return operator delete(Memory, PoolType, PoolTag);
}



class A
{
    int x = 0;
    int y = 0;
public:
    void set(int _x)
    {
        x = _x;
    }

    virtual ~A()
    {
        x = 0;
        y = 10;
    }
};

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);

    auto a = A();

    auto v = std::vector<int>();

    
    a.set([&]() -> int
        {
            return 0;
        }());;

    a.set(5);

    return STATUS_UNSUCCESSFUL;
}