#include "ruby.h"
#include <string>
#include <iostream>

#include "shp.hpp"
#include "shapefile.hpp"
#include "dbf.hpp"
#include "shape_object.hpp"

VALUE cSHP;

extern "C" {
  void Init_shp() {
    cSHP = rb_define_module("SHP");
    shp::shapefile::define(cSHP);
    shp::dbf::define(cSHP);
    shp::shape_object::define(cSHP);
  }
}
