#include <ntifs.h>

extern unsigned long crt_init(void* hal_context);
extern void crt_shutdown(void* hal_context);

typedef struct _HalContext
{
    PDRIVER_OBJECT DriverObject;
    PUNICODE_STRING RegistryPath;
} HalContext, * PHalContext;

static HalContext hal_context = { 0 };

void DriverUnload(PDRIVER_OBJECT drvobj)
{
    crt_shutdown(&hal_context);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    hal_context.DriverObject = DriverObject;
    hal_context.RegistryPath = RegistryPath;
    
    DriverObject->DriverUnload = DriverUnload;

    return crt_init(&hal_context);
}
