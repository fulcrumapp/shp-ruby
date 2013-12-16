
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

  CHECK_VALID_HANDLE(object->value());

  SHPComputeExtents(object->value());

  return object->wrapped();
}

VALUE shape_object::destroy(VALUE self)
{
  shape_object *object = unwrap(self);

  CHECK_VALID_HANDLE(object->value());

  if (object->value()) {
    SHPDestroyObject(object->value());
    object->_handle = NULL;
  }

  return Qnil;
}

VALUE shape_object::get_shape_type(VALUE self)
{
  shape_object *object = unwrap(self);

  CHECK_VALID_HANDLE(object->value());

  return INT2FIX(object->value()->nSHPType);
}

VALUE shape_object::get_shape_id(VALUE self)
{
  shape_object *object = unwrap(self);

  CHECK_VALID_HANDLE(object->value());

  return INT2FIX(object->value()->nShapeId);
}

/* VALUE shape_object::get_shape_id(VALUE self); */
/* VALUE shape_object::get_shape_parts(VALUE self); */
/* VALUE shape_object::get_shape_part_starts(VALUE self); */
/* VALUE shape_object::get_shape_part_types(VALUE self); */
/* VALUE shape_object::get_vertex_count(VALUE self); */
/* VALUE shape_object::get_x(VALUE self); */
/* VALUE shape_object::get_y(VALUE self); */
/* VALUE shape_object::get_z(VALUE self); */
/* VALUE shape_object::get_m(VALUE self); */
/* VALUE shape_object::get_x_min(VALUE self); */
/* VALUE shape_object::get_y_min(VALUE self); */
/* VALUE shape_object::get_z_min(VALUE self); */
/* VALUE shape_object::get_m_min(VALUE self); */
/* VALUE shape_object::get_x_max(VALUE self); */
/* VALUE shape_object::get_y_max(VALUE self); */
/* VALUE shape_object::get_z_max(VALUE self); */
/* VALUE shape_object::get_m_max(VALUE self); */

void shape_object::define(VALUE module)
{
  shape_object::_klass = rb_define_class_under(module, "ShapeObject", rb_cObject);
  base::define(shape_object::_klass, false);
  rb_define_method(shape_object::_klass, "compute_extents", SHP_METHOD(shape_object::compute_extents), 0);
  rb_define_method(shape_object::_klass, "get_shape_type", SHP_METHOD(shape_object::get_shape_type), 0);
  rb_define_method(shape_object::_klass, "get_shape_id", SHP_METHOD(shape_object::get_shape_id), 0);
  rb_define_method(shape_object::_klass, "destroy", SHP_METHOD(shape_object::destroy), 0);
}

}

