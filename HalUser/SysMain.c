#include <windows.h>

extern unsigned long crt_init(void* hal_context);
extern void crt_shutdown(void* hal_context);

int main()
{
    int result = crt_init(NULL);
    if (0 == result)
    {
        crt_shutdown(NULL);
    }

    return result;

    // in a DLL we will have an unload
}
