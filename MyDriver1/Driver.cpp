#include <ucxxrt.h>
#include <kext/kallocator.h>

#define LOG(Format, ...) DbgPrintEx(DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, "[ucxxrt] [" __FUNCTION__ ":%u]: " Format "\n", __LINE__, ## __VA_ARGS__)

namespace Main
{
    EXTERN_C NTSTATUS ModuleMain(unsigned long state, void* context)
    {
        UNREFERENCED_PARAMETER(context);

        switch (state)
        {
        case 1:
            LOG("HI");
            break;
        case 0:
            LOG("BYE");
            break;
        }

        return 0l;
    }

}
