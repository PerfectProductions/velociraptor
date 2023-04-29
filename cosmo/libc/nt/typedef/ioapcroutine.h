#ifndef COSMOPOLITAN_LIBC_NT_TYPEDEF_IOAPCROUTINE_H_
#define COSMOPOLITAN_LIBC_NT_TYPEDEF_IOAPCROUTINE_H_
#include "libc/nt/struct/iostatusblock.h"
#if !(__ASSEMBLER__ + __LINKER__ + 0)

typedef void (*NtIoApcRoutine)(void *ApcContext,
                               struct NtIoStatusBlock *IoStatusBlock,
                               uint32_t Reserved);

#endif /* !(__ASSEMBLER__ + __LINKER__ + 0) */
#endif /* COSMOPOLITAN_LIBC_NT_TYPEDEF_IOAPCROUTINE_H_ */
