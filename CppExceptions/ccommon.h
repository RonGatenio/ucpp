#pragma once

#include <Windows.h>
//#include <ntstatus.h>


// end_ntddk end_wdm end_nthal end_ntminiport

//
// Define AMD64 exception handling structures and function prototypes.
//
// Define unwind operation codes.
//

typedef enum _UNWIND_OP_CODES {
    UWOP_PUSH_NONVOL = 0,
    UWOP_ALLOC_LARGE,
    UWOP_ALLOC_SMALL,
    UWOP_SET_FPREG,
    UWOP_SAVE_NONVOL,
    UWOP_SAVE_NONVOL_FAR,
    UWOP_SPARE_CODE1,
    UWOP_SPARE_CODE2,
    UWOP_SAVE_XMM128,
    UWOP_SAVE_XMM128_FAR,
    UWOP_PUSH_MACHFRAME
} UNWIND_OP_CODES, * PUNWIND_OP_CODES;

//
// Define unwind code structure.
//

typedef union _UNWIND_CODE {
    struct {
        UCHAR CodeOffset;
        UCHAR UnwindOp : 4;
        UCHAR OpInfo : 4;
    };

    USHORT FrameOffset;
} UNWIND_CODE, * PUNWIND_CODE;

//
// Define unwind information flags.
//

#define UNW_FLAG_NHANDLER 0x0
#define UNW_FLAG_EHANDLER 0x1
#define UNW_FLAG_UHANDLER 0x2
#define UNW_FLAG_CHAININFO 0x4

//
// Define unwind information structure.
//

typedef struct _UNWIND_INFO {
    UCHAR Version : 3;
    UCHAR Flags : 5;
    UCHAR SizeOfProlog;
    UCHAR CountOfCodes;
    UCHAR FrameRegister : 4;
    UCHAR FrameOffset : 4;
    UNWIND_CODE UnwindCode[1];

    //
    // The unwind codes are followed by an optional DWORD aligned field that
    // contains the exception handler address or a function table entry if
    // chained unwind information is specified. If an exception handler address
    // is specified, then it is followed by the language specified exception
    // handler data.
    //
    //  union {
    //      struct {
    //          ULONG ExceptionHandler;
    //          ULONG ExceptionData[];
    //      };
    //
    //      RUNTIME_FUNCTION FunctionEntry;
    //  };
    //

} UNWIND_INFO, * PUNWIND_INFO;

// begin_winnt
//
// Define function table entry - a function table entry is generated for
// each frame function.
//

#define RUNTIME_FUNCTION_INDIRECT 0x1


#define UNWIND_HISTORY_TABLE_SIZE 12
#define UNWIND_HISTORY_TABLE_NONE 0
#define UNWIND_HISTORY_TABLE_GLOBAL 1
#define UNWIND_HISTORY_TABLE_LOCAL 2



//
// Logical Data Type - These are 32-bit logical values.
//

typedef ULONG LOGICAL;
typedef ULONG* PLOGICAL;


//////////////////////////////////////////////////////
#define KernelMode                              0x0
#define UserMode                                0x1

#define ULONG64_MAX (~(ULONG64)0)

typedef LONG NTSTATUS;
#define STATUS_BAD_STACK                        ((NTSTATUS)0xC0000028)
#define STATUS_BAD_FUNCTION_TABLE               ((NTSTATUS)0xC00000FF)
#define STATUS_UNWIND                           ((NTSTATUS)0xC0000027)
#define  ASSERT(f) if (!f) {__debugbreak();}

//
// Determine if an argument is present by testing the value of the pointer
// to the argument value.
//

#define ARGUMENT_PRESENT(ArgumentPointer)    (\
    (CHAR *)((ULONG_PTR)(ArgumentPointer)) != (CHAR *)(NULL) )

#define offsetof(s,m) ((size_t)&(((s*)0)->m))
#define RVA(m, b) ((PVOID)((ULONG_PTR)(b) + (ULONG_PTR)(m)))

//
// Alignment Macros
//
#define ALIGN_DOWN_BY(size, align) \
    ((ULONG_PTR)(size) & ~((ULONG_PTR)(align) - 1))
#define ALIGN_UP_BY(size, align) \
    (ALIGN_DOWN_BY(((ULONG_PTR)(size) + align - 1), align))
#define ALIGN_DOWN_POINTER_BY(ptr, align) \
    ((PVOID)ALIGN_DOWN_BY(ptr, align))
#define ALIGN_UP_POINTER_BY(ptr, align) \
    ((PVOID)ALIGN_UP_BY(ptr, align))
#define ALIGN_DOWN(size, type) \
    ALIGN_DOWN_BY(size, sizeof(type))
#define ALIGN_UP(size, type) \
    ALIGN_UP_BY(size, sizeof(type))
#define ALIGN_DOWN_POINTER(ptr, type) \
    ALIGN_DOWN_POINTER_BY(ptr, sizeof(type))
#define ALIGN_UP_POINTER(ptr, type) \
    ALIGN_UP_POINTER_BY(ptr, sizeof(type))