
#include "dbf.hpp"

namespace shp {

VALUE dbf::_klass = Qnil;

VALUE dbf::klass() {
  return dbf::_klass;
}

dbf::dbf(DBFHandle handle)
  : _handle(handle)
{
}

dbf::~dbf() {
  if (_handle) {
    DBFClose(_handle);
    _handle = NULL;
  }
}

VALUE dbf::initialize(int argc, VALUE *argv)
{
  return initialize_not_allowed();
}

VALUE dbf::create(VALUE klass, VALUE filename)
{
  CHECK_ARGUMENT_STRING(filename);

  DBFHandle handle = DBFCreate(RSTRING_PTR(filename));

  if (handle == NULL) {
    SHP_FATAL("Failed to create dbf file.");
    return Qnil;
  }

  dbf *file = new dbf(handle);

  return file->wrapped();
}

VALUE dbf::open(VALUE klass, VALUE filename, VALUE access)
{
  CHECK_ARGUMENT_STRING(filename);
  CHECK_ARGUMENT_STRING(access);

  DBFHandle handle = DBFOpen(RSTRING_PTR(filename), RSTRING_PTR(access));

  if (handle == NULL) {
    SHP_FATAL("Failed to open dbf file.");
    return Qnil;
  }

  dbf *file = new dbf(handle);

  return file->wrapped();
}

VALUE dbf::add_field(VALUE self, VALUE fieldName, VALUE fieldType, VALUE fieldWidth, VALUE fieldDecimals)
{
  CHECK_ARGUMENT_STRING(fieldName);
  CHECK_ARGUMENT_FIXNUM(fieldType);
  CHECK_ARGUMENT_FIXNUM(fieldWidth);
  CHECK_ARGUMENT_FIXNUM(fieldDecimals);

  dbf *db = unwrap(self);

  std::string theFieldName(RSTRING_PTR(fieldName));

  int result = DBFAddField(db->value(),
                           theFieldName.c_str(),
                           (DBFFieldType)FIX2INT(fieldType),
                           FIX2INT(fieldWidth),
                           FIX2INT(fieldDecimals));

  if (result == -1) {
    SHP_FATAL("Failed to add new field");
    return Qnil;
  }

  return INT2FIX(result);
}

VALUE dbf::write_integer_attribute(VALUE self, VALUE recordNumber, VALUE fieldIndex, VALUE fieldValue)
{
  CHECK_ARGUMENT_FIXNUM(recordNumber);
  CHECK_ARGUMENT_FIXNUM(fieldIndex);
  CHECK_ARGUMENT_FIXNUM(fieldValue);

  dbf *db = unwrap(self);

  int result = DBFWriteIntegerAttribute(db->value(),
                                        FIX2INT(recordNumber),
                                        FIX2INT(fieldIndex),
                                        FIX2INT(fieldValue));

  if (result == 0) {
    SHP_FATAL("Failed to write new attribute.");
    return Qnil;
  }

  return INT2FIX(result);
}

VALUE dbf::write_double_attribute(VALUE self, VALUE recordNumber, VALUE fieldIndex, VALUE fieldValue)
{
  CHECK_ARGUMENT_FIXNUM(recordNumber);
  CHECK_ARGUMENT_FIXNUM(fieldIndex);
  CHECK_ARGUMENT_FLOAT(fieldValue);

  dbf *db = unwrap(self);

  int result = DBFWriteDoubleAttribute(db->value(),
                                       FIX2INT(recordNumber),
                                       FIX2INT(fieldIndex),
                                       NUM2DBL(fieldValue));

  if (result == 0) {
    SHP_FATAL("Failed to write new attribute.");
    return Qnil;
  }

  return INT2FIX(result);
}

VALUE dbf::write_string_attribute(VALUE self, VALUE recordNumber, VALUE fieldIndex, VALUE fieldValue)
{
  CHECK_ARGUMENT_FIXNUM(recordNumber);
  CHECK_ARGUMENT_FIXNUM(fieldIndex);
  CHECK_ARGUMENT_STRING(fieldValue);

  dbf *db = unwrap(self);

  int result = DBFWriteStringAttribute(db->value(),
                                       FIX2INT(recordNumber),
                                       FIX2INT(fieldIndex),
                                       RSTRING_PTR(fieldValue));

  if (result == 0) {
    SHP_FATAL("Failed to write new attribute.");
    return Qnil;
  }

  return INT2FIX(result);
}

VALUE dbf::write_null_attribute(VALUE self, VALUE recordNumber, VALUE fieldIndex)
{
  CHECK_ARGUMENT_FIXNUM(recordNumber);
  CHECK_ARGUMENT_FIXNUM(fieldIndex);

  dbf *db = unwrap(self);

  int result = DBFWriteNULLAttribute(db->value(),
                                     FIX2INT(recordNumber),
                                     FIX2INT(fieldIndex));

  if (result == 0) {
    SHP_FATAL("Failed to write new attribute.");
    return Qnil;
  }

  return INT2FIX(result);
}

VALUE dbf::close(VALUE self)
{
  dbf *db = unwrap(self);

  DBFClose(db->value());

  db->_handle = NULL;

  return Qnil;
}

void dbf::define(VALUE module)
{
  dbf::_klass = rb_define_class_under(module, "DBF", rb_cObject);
  base::define(dbf::_klass);
  rb_define_singleton_method(dbf::_klass, "create", SHP_METHOD(dbf::create), 1);
  rb_define_singleton_method(dbf::_klass, "open", SHP_METHOD(dbf::open), 2);
  rb_define_method(dbf::_klass, "add_field", SHP_METHOD(dbf::add_field), 4);
  rb_define_method(dbf::_klass, "write_integer_attribute", SHP_METHOD(dbf::write_integer_attribute), 3);
  rb_define_method(dbf::_klass, "write_double_attribute", SHP_METHOD(dbf::write_double_attribute), 3);
  rb_define_method(dbf::_klass, "write_string_attribute", SHP_METHOD(dbf::write_string_attribute), 3);
  rb_define_method(dbf::_klass, "write_null_attribute", SHP_METHOD(dbf::write_null_attribute), 2);
  rb_define_method(dbf::_klass, "close", SHP_METHOD(dbf::close), 0);
}

}

