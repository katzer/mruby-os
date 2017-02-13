/*
 * Copyright (c) 2016 by appPlant GmbH. All rights reserved.
 *
 * @APPPLANT_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apache License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://opensource.org/licenses/Apache-2.0/ and read it before using this
 * file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPPLANT_LICENSE_HEADER_END@
 */

#include "mruby.h"
#include "mruby/string.h"
#include <string.h>

#ifdef _WIN32
struct utsname
{
  const char *sysname;
  const char *machine;
} utsname_windows = { "Windows NT", "x86_64" };

int uname(struct utsname*);
#else
#include <sys/utsname.h>
#endif /* UTSNAME_H */

/**
 * Struct with details about the sysname and machine.
 */
static struct utsname
mrb_os_uname(mrb_state *mrb)
{
  struct utsname buffer;

#if defined(_WIN32) || defined(_WIN64)
  buffer = utsname_windows;
#else
  if (uname(&buffer) != 0) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "uname failed");
  }
#endif
  return buffer;
}

/**
 * Returns the pointer size used by OS or the binary.
 *
 *  OS.bits or OS.bits(:os)  -> Fixnum
 *             OS.bits(:bin) -> Fixnum
 */
static mrb_value
mrb_os_bits(mrb_state *mrb, mrb_value self)
{
  mrb_value arg;
  int argc;
  const char* str = "machine";

  argc = mrb_get_args(mrb, "|o", &arg);

  if (argc != 0) {
    str = mrb_string_value_cstr(mrb, &arg);
  }

  if (strcmp(str, "os") == 0) {
  #if defined(__x86_64__) || defined(_WIN64)
    return mrb_fixnum_value(64);
  #elif defined(__i386) || defined(_WIN32)
    return mrb_fixnum_value(32);
  #endif
  }

  if (strcmp(str, "binary") == 0) {
    return mrb_fixnum_value(8 * sizeof(void*));
  }

  return mrb_nil_value();
}

/**
 * Returns the architectur of the machine.
 *
 *  OS.machine -> String ('x86_64' or 'i686')
 */
static mrb_value
mrb_os_machine(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, mrb_os_uname(mrb).machine);
}

/**
 * Returns the sysname of the host.
 *
 *  OS.sysname -> String
 */
static mrb_value
mrb_os_sysname(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, mrb_os_uname(mrb).sysname);
}

/**
 * Returns true if binary is running on OSX.
 *
 *  OS.mac? -> Bool
 *  OS.osx? -> Bool
 *  OS.x? -> Bool
 */
static mrb_value
mrb_os_mac(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(strcmp(mrb_os_uname(mrb).sysname, "Darwin") == 0);
}

/**
 * Returns true if binary is running on Linux.
 *
 *  OS.linux? -> BOOL
 */
static mrb_value
mrb_os_linux(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(strcmp(mrb_os_uname(mrb).sysname, "Linux") == 0);
}

/**
 * Returns true if binary is running on Windows.
 *
 *  OS.windows? -> BOOL
 */
static mrb_value
mrb_os_windows(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(strcmp(mrb_os_uname(mrb).sysname, "Windows NT") == 0);
}

/**
 * Returns true if binary is running on a POSIX-like system.
 *
 *  OS.windows? -> BOOL
 */
static mrb_value
mrb_os_posix(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(strcmp(mrb_os_uname(mrb).sysname, "Windows NT") != 0);
}

void
mrb_mruby_os_gem_init(mrb_state* mrb)
{
  struct RClass *os;

  os = mrb_define_module(mrb, "OS");

  mrb_define_class_method(mrb, os, "bits", mrb_os_bits, MRB_ARGS_OPT(1));
  mrb_define_class_method(mrb, os, "machine", mrb_os_machine, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, os, "sysname", mrb_os_sysname, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, os, "mac?", mrb_os_mac, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, os, "osx?", mrb_os_mac, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, os, "x?", mrb_os_mac, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, os, "linux?", mrb_os_linux, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, os, "windows?", mrb_os_windows, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, os, "posix?", mrb_os_posix, MRB_ARGS_NONE());
}

void
mrb_mruby_os_gem_final(mrb_state* mrb)
{
  // Nothing to do here
}
