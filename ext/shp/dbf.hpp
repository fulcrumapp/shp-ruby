#ifndef __SHP_DBF_HPP__
#define __SHP_DBF_HPP__

#include "shp.hpp"
#include "base.hpp"

namespace shp {
  class dbf : public base<dbf> {
  public:
    dbf() : base<dbf>(), _handle(0) {};
    dbf(DBFHandle handle);
    virtual VALUE initialize(int argc, VALUE *argv);
    static void define(VALUE module);
    static VALUE create(VALUE klass, VALUE filename);
    static VALUE open(VALUE klass, VALUE filename, VALUE access);
    static VALUE add_field(VALUE self, VALUE fieldName, VALUE fieldType, VALUE fieldWidth, VALUE fieldDecimals);
    static VALUE write_integer_attribute(VALUE self, VALUE recordNumber, VALUE fieldIndex, VALUE fieldValue);
    static VALUE write_double_attribute(VALUE self, VALUE recordNumber, VALUE fieldIndex, VALUE fieldValue);
    static VALUE write_string_attribute(VALUE self, VALUE recordNumber, VALUE fieldIndex, VALUE fieldValue);
    static VALUE write_null_attribute(VALUE self, VALUE recordNumber, VALUE fieldIndex);
    static VALUE read_integer_attribute(VALUE self, VALUE recordIndex, VALUE fieldIndex);
    static VALUE read_double_attribute(VALUE self, VALUE recordIndex, VALUE fieldIndex);
    static VALUE read_string_attribute(VALUE self, VALUE recordIndex, VALUE fieldIndex);
    static VALUE is_attribute_null(VALUE self, VALUE recordIndex, VALUE fieldIndex);
    static VALUE close(VALUE self);
    static VALUE get_field_count(VALUE self);
    static VALUE get_record_count(VALUE self);
    static VALUE get_field_index(VALUE self, VALUE fieldName);
    static VALUE get_field_info(VALUE self, VALUE fieldIndex);
    static VALUE is_record_deleted(VALUE self, VALUE recordIndex);
    static VALUE mark_record_deleted(VALUE self, VALUE recordIndex, VALUE isDeleted);

    virtual VALUE klass();
    static VALUE _klass;

    DBFHandle value() { return _handle; }

    virtual ~dbf();
  private:
    DBFHandle _handle;
  };
}

#endif
