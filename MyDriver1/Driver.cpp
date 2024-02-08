#include <ucxxrt.h>
#include <kext/kallocator.h>

#define LOG(Format, ...) DbgPrintEx(DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, "[ucxxrt] [" __FUNCTION__ ":%u]: " Format "\n", __LINE__, ## __VA_ARGS__)

namespace Main
{
    EXTERN_C NTSTATUS DriverMain(const PDRIVER_OBJECT DriverObject, const PUNICODE_STRING Registry)
    {
        UNREFERENCED_PARAMETER(Registry);

        LOG("HI");

        DriverObject->DriverUnload = [](auto)
        {
            LOG("BYE");
        };

        return 0l;
    }

}
