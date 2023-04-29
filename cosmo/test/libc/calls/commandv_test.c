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
#include "libc/calls/calls.h"
#include "libc/dce.h"
#include "libc/mem/mem.h"
#include "libc/runtime/gc.h"
#include "libc/runtime/internal.h"
#include "libc/runtime/runtime.h"
#include "libc/stdio/stdio.h"
#include "libc/testlib/testlib.h"
#include "libc/x/x.h"

uint64_t i;
char pathbuf[PATH_MAX];
const char *oldpath, *bindir, *homedir, *binsh, *sh;

TEST(commandv_00, todo) { /* TODO(jart): Improve this on Windows. */
  if (IsWindows()) exit(0);
}

TEST(commandv_001, setupFiles) {
  mkdir("o", 0755);
  mkdir("o/tmp", 0755);
  oldpath = strdup(getenv("PATH"));
  homedir = xasprintf("o/tmp/home.%d", getpid());
  bindir = xasprintf("o/tmp/bin.%d", getpid());
  binsh = xasprintf("%s/sh.com", bindir);
  ASSERT_NE(-1, mkdir(homedir, 0755));
  ASSERT_NE(-1, mkdir(bindir, 0755));
  ASSERT_NE(-1, touch(binsh, 0755));
  ASSERT_NE(-1, setenv("PATH", bindir, true));
}

TEST(commandv_010, testSlashes_wontSearchPath_butChecksAccess) {
  sh = defer(unlink, gc(xasprintf("%s/sh.com", homedir)));
  EXPECT_NE(-1, touch(sh, 0755));
  i = g_syscount;
  EXPECT_STREQ(sh, commandv(sh, pathbuf));
  if (!IsWindows()) EXPECT_EQ(i + 1 /* access() */, g_syscount);
}

TEST(commandv_999, teardown) {
  setenv("PATH", oldpath, true);
  unlink(binsh);
  rmdir(bindir);
  rmdir(homedir);
  free(bindir);
  free(binsh);
  free(homedir);
  free(oldpath);
}
