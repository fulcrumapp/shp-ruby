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
    static VALUE compute_extents(VALUE self);
    static VALUE destroy(VALUE self);

    static VALUE get_shape_type(VALUE self);
    static VALUE get_shape_id(VALUE self);
    static VALUE get_shape_parts(VALUE self);
    static VALUE get_shape_part_starts(VALUE self);
    static VALUE get_shape_part_types(VALUE self);
    static VALUE get_vertex_count(VALUE self);
    static VALUE get_x(VALUE self);
    static VALUE get_y(VALUE self);
    static VALUE get_z(VALUE self);
    static VALUE get_m(VALUE self);
    static VALUE get_x_min(VALUE self);
    static VALUE get_y_min(VALUE self);
    static VALUE get_z_min(VALUE self);
    static VALUE get_m_min(VALUE self);
    static VALUE get_x_max(VALUE self);
    static VALUE get_y_max(VALUE self);
    static VALUE get_z_max(VALUE self);
    static VALUE get_m_max(VALUE self);

    virtual VALUE klass();
    static VALUE _klass;

    SHPObject *value() { return _handle; }

    virtual ~shape_object();
  private:
    SHPObject *_handle;
  };
}

#endif
