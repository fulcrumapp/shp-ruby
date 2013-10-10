
#include "shape_object.hpp"

namespace shp {

VALUE shape_object::_klass = Qnil;

VALUE shape_object::klass() {
  return shape_object::_klass;
}

shape_object::shape_object(SHPObject *handle)
  : _handle(handle)
{
}

shape_object::~shape_object() {
  if (_handle) {
    SHPDestroyObject(_handle);
    _handle = NULL;
  }
}

void shape_object::define(VALUE module)
{
  shape_object::_klass = rb_define_class_under(module, "ShapeObject", rb_cObject);
  base::define(shape_object::_klass);
}

}

