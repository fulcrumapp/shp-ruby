#ifndef __SHP_SHAPEFILE_HPP__
#define __SHP_SHAPEFILE_HPP__

#include "shp.hpp"
#include "base.hpp"

namespace shp {
  class shapefile : public base<shapefile> {
  public:
    shapefile() : base<shapefile>(), _handle(0) {};
    shapefile(SHPHandle handle);
    virtual VALUE initialize(int argc, VALUE *argv);
    static void define(VALUE module);
    static VALUE create(VALUE klass, VALUE filename, VALUE shapeType);
    static VALUE create_simple_object(VALUE self, VALUE shapeType, VALUE numberOfVertices, VALUE arrayOfX, VALUE arrayOfY, VALUE arrayOfZ);
    static VALUE write_object(VALUE self, VALUE shapeIndex, VALUE shapeObject);
    static VALUE close(VALUE self);
    static VALUE get_info(VALUE self);
    static VALUE read_object(VALUE self, VALUE shapeIndex);

    virtual VALUE klass();
    static VALUE _klass;

    SHPHandle value() { return _handle; }

    virtual ~shapefile();
  private:
    SHPHandle _handle;
  };
}

#endif
