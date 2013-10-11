#ifndef __SHP_HPP__
#define __SHP_HPP__

#include "ruby.h"
#include <string>
#include <iostream>
#include <shapefil.h>

using namespace std;

typedef VALUE (*RB_INSTANCE_METHOD)(...);

#define SHP_FATAL(msg) rb_raise(rb_eRuntimeError, msg)
#define SHP_FATAL_ARGUMENT(msg) rb_raise(rb_eArgError, msg)
#define SHP_METHOD(method) ((RB_INSTANCE_METHOD)&method)

#define CHECK_ARGUMENT_STRING(arg) \
  if (TYPE(arg) != T_STRING) { \
    SHP_FATAL_ARGUMENT("Argument must be a String"); \
    return Qnil; \
  }

#define CHECK_ARGUMENT_FIXNUM(arg) \
  if (TYPE(arg) != T_FIXNUM) { \
    SHP_FATAL_ARGUMENT("Argument must be a FixNum"); \
    return Qnil; \
  }

#define CHECK_ARGUMENT_FLOAT(arg) \
  if (TYPE(arg) != T_FLOAT) { \
    SHP_FATAL_ARGUMENT("Argument must be a Float"); \
    return Qnil; \
  }

#define CHECK_ARGUMENT_ARRAY(arg) \
  if (TYPE(arg) != T_ARRAY) { \
    SHP_FATAL_ARGUMENT("Argument must be an Array"); \
    return Qnil; \
  }

#endif
