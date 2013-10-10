#ifndef __SHP_SHAPE_OBJECT_HPP__
#define __SHP_SHAPE_OBJECT_HPP__

#include "shp.hpp"
#include "base.hpp"

namespace shp {
  class shape_object : public base<shape_object> {
  public:
    shape_object() : base<shape_object>(), _handle(0) {};
    shape_object(SHPObject *handle);
    static void define(VALUE module);

    virtual VALUE klass();
    static VALUE _klass;

    SHPObject *value() { return _handle; }

    virtual ~shape_object();
  private:
    SHPObject *_handle;
  };
}

#endif
