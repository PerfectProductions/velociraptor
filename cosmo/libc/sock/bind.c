/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2020 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/calls/internal.h"
#include "libc/dce.h"
#include "libc/sock/internal.h"
#include "libc/sock/sock.h"
#include "libc/str/str.h"
#include "libc/sysv/errfuns.h"

/**
 * Assigns local address and port number to socket.
 *
 * @param fd is the file descriptor returned by socket()
 * @param addr is usually the binary-encoded ip:port on which to listen
 * @param addrsize is the byte-length of addr's true polymorphic form
 * @return socket file descriptor or -1 w/ errno
 * @error ENETDOWN, EPFNOSUPPORT, etc.
 * @asyncsignalsafe
 */
int bind(int fd, const void *addr, uint32_t addrsize) {
  if (!addr) return efault();
  if (addrsize == sizeof(struct sockaddr_in)) {
    if (!IsWindows()) {
      if (!IsBsd()) {
        return bind$sysv(fd, addr, addrsize);
      } else {
        struct sockaddr_in$bsd addr2;
        _Static_assert(sizeof(struct sockaddr_in) ==
                       sizeof(struct sockaddr_in$bsd));
        memcpy(&addr2, addr, sizeof(struct sockaddr_in));
        sockaddr2bsd(&addr2);
        return bind$sysv(fd, &addr2, addrsize);
      }
    } else if (__isfdkind(fd, kFdSocket)) {
      return bind$nt(&g_fds.p[fd], addr, addrsize);
    } else {
      return ebadf();
    }
  } else {
    return einval();
  }
}
