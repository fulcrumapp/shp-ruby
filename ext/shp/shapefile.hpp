#ifndef __SHP_SHAPEFILE_HPP__
#define __SHP_SHAPEFILE_HPP__

#include "shp.hpp"
#include "base.hpp"

namespace shp {
  class shapefile : public base<shapefile> {
  public:
    shapefile() : base<shapefile>(), _handle(0) {};
    shapefile(SHPHandle handle);
    static void define(VALUE module);
    static VALUE create(VALUE klass, VALUE filename, VALUE shapeType);
    static VALUE create_simple_object(VALUE klass, VALUE shapeType, VALUE numberOfVertices, VALUE arrayOfX, VALUE arrayOfY, VALUE arrayOfZ);
    static VALUE create_object(VALUE klass, VALUE shapeType, VALUE shapeIndex, VALUE numberOfParts,
                               VALUE arrayOfPartStarts, VALUE arrayOfPartTypes, VALUE numberOfVertices,
                               VALUE arrayOfX, VALUE arrayOfY, VALUE arrayOfZ, VALUE arrayOfM);
    static VALUE write_object(VALUE self, VALUE shapeIndex, VALUE shapeObject);
    static VALUE close(VALUE self);
    static VALUE get_info(VALUE self);
    static VALUE read_object(VALUE self, VALUE shapeIndex);
    static VALUE rewind_object(VALUE self, VALUE shapeObject);

    virtual VALUE klass();
    static VALUE _klass;

    SHPHandle value() { return _handle; }

    virtual ~shapefile();
  private:
    SHPHandle _handle;
  };
}

#endif
