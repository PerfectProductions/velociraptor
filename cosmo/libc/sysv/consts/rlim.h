#ifndef COSMOPOLITAN_LIBC_SYSV_CONSTS_RLIM_H_
#define COSMOPOLITAN_LIBC_SYSV_CONSTS_RLIM_H_
#include "libc/runtime/symbolic.h"

#define RLIM_INFINITY SYMBOLIC(RLIM_INFINITY)
#define RLIM_NLIMITS SYMBOLIC(RLIM_NLIMITS)
#define RLIM_SAVED_CUR SYMBOLIC(RLIM_SAVED_CUR)
#define RLIM_SAVED_MAX SYMBOLIC(RLIM_SAVED_MAX)

#if !(__ASSEMBLER__ + __LINKER__ + 0)
COSMOPOLITAN_C_START_

hidden extern const long RLIM_INFINITY;
hidden extern const long RLIM_NLIMITS;
hidden extern const long RLIM_SAVED_CUR;
hidden extern const long RLIM_SAVED_MAX;

COSMOPOLITAN_C_END_
#endif /* !(__ASSEMBLER__ + __LINKER__ + 0) */
#endif /* COSMOPOLITAN_LIBC_SYSV_CONSTS_RLIM_H_ */
