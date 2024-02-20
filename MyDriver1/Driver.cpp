#include <ucxxrt.h>
#include <ntifs.h>
#include <kext/kallocator.h>

#define LOG(Format, ...) DbgPrintEx(DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, "[ucxxrt] [" __FUNCTION__ ":%u]: " Format "\n", __LINE__, ## __VA_ARGS__)

class A
{
    int x;
public:
    explicit A(int z) : x(z) {}
    void set_x(int y)
    {
        x = y;
    }
    int get_x()
    {
        return x;
    }
};

A g_a(5);

namespace Main
{
    extern "C" NTSTATUS ModuleMain(unsigned long state, void* context)
    {
        UNREFERENCED_PARAMETER(context);

        switch (state)
        {
        case 1:
            LOG("HI %d", g_a.get_x());
            break;
        case 0:
            LOG("BYE");
            break;
        }

        return 0l;
    }

}
