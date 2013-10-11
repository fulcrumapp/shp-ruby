#ifndef __SHP_BASE_HPP__
#define __SHP_BASE_HPP__

#include "shp.hpp"

namespace shp {
  template <typename T> class base {
  public:
    base<T>() : _value(0) {}

    static VALUE initialize_instance(int argc, VALUE *argv, VALUE self) {
      return unwrap(self)->initialize(argc, argv);
    }

    static VALUE new_instance(int argc, VALUE *argv, VALUE klass) {
      T *instance = new T();
      VALUE rvalue = instance->wrapped();
      rb_obj_call_init(rvalue, argc, argv);
      return rvalue;
    }

    static VALUE new_instance_not_allowed(int argc, VALUE *argv, VALUE klass) {
      rb_raise(rb_eRuntimeError, "You cannot use #new on this class.");
      return Qnil;
    }

    static VALUE initialize_instance_not_allowed(int argc, VALUE *argv, VALUE klass) {
      rb_raise(rb_eRuntimeError, "You cannot use #initialize on this class.");
      return Qnil;
    }

    VALUE wrapped() {
      if (!_value) {
        _value = Data_Wrap_Struct(klass(), base<T>::mark, base<T>::dispose, this);
      }
      return _value;
    }

    void mark_self() {
      if (_value) {
        rb_gc_mark(_value);
      }
    }

    virtual void mark() {}
    virtual void dispose() {}
    virtual VALUE initialize(int argc, VALUE *argv) { return rvalue(); }

    virtual VALUE klass() {
      return Qnil;
    }

    virtual ~base() {}

    VALUE rvalue() { return _value; }

    static T *unwrap(VALUE self) {
      T *obj = NULL;
      Data_Get_Struct(self, T, obj);
      return obj;
    }

  protected:
    VALUE _value;

    static base<T> *object(void *ptr) {
      return (base *)ptr;
    }

    static void mark(void *ptr) {
      base::object(ptr)->mark();
    }

    static void dispose(void *ptr) {
      delete base::object(ptr);
    }

    static void define(VALUE klass, bool allowNew) {
      if (allowNew) {
        rb_define_singleton_method(klass, "new", SHP_METHOD(base<T>::new_instance), -1);
        rb_define_method(klass, "initialize", SHP_METHOD(base<T>::initialize_instance), -1);
      }
      else {
        rb_define_singleton_method(klass, "new", SHP_METHOD(base<T>::new_instance_not_allowed), -1);
        rb_define_method(klass, "initialize", SHP_METHOD(base<T>::initialize_instance_not_allowed), -1);
      }
    }

    VALUE initialize_not_allowed()
    {
      rb_raise(rb_eRuntimeError, "You cannot use #new on this class.");
      return Qnil;
    }

  };
}

#endif
