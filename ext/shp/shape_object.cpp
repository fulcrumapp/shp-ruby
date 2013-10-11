
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

VALUE shape_object::compute_extents(VALUE self)
{
  shape_object *object = unwrap(self);

  SHPComputeExtents(object->value());

  return object->wrapped();
}

void shape_object::define(VALUE module)
{
  shape_object::_klass = rb_define_class_under(module, "ShapeObject", rb_cObject);
  base::define(shape_object::_klass, false);
  rb_define_method(shape_object::_klass, "compute_extents", SHP_METHOD(shape_object::compute_extents), 0);
}

}

