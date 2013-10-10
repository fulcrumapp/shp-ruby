
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

VALUE dbf::read_integer_attribute(VALUE self, VALUE recordIndex, VALUE fieldIndex)
{
  CHECK_ARGUMENT_FIXNUM(recordIndex);
  CHECK_ARGUMENT_FIXNUM(fieldIndex);

  dbf *db = unwrap(self);

  int result = DBFReadIntegerAttribute(db->value(),
                                       FIX2INT(recordIndex),
                                       FIX2INT(fieldIndex));

  return INT2FIX(result);
}

VALUE dbf::read_double_attribute(VALUE self, VALUE recordIndex, VALUE fieldIndex)
{
  CHECK_ARGUMENT_FIXNUM(recordIndex);
  CHECK_ARGUMENT_FIXNUM(fieldIndex);

  dbf *db = unwrap(self);

  double result = DBFReadDoubleAttribute(db->value(),
                                         FIX2INT(recordIndex),
                                         FIX2INT(fieldIndex));

  return rb_float_new(result);
}

VALUE dbf::read_string_attribute(VALUE self, VALUE recordIndex, VALUE fieldIndex)
{
  CHECK_ARGUMENT_FIXNUM(recordIndex);
  CHECK_ARGUMENT_FIXNUM(fieldIndex);

  dbf *db = unwrap(self);

  const char *result = DBFReadStringAttribute(db->value(),
                                              FIX2INT(recordIndex),
                                              FIX2INT(fieldIndex));

  return rb_str_new2(result);
}

VALUE dbf::is_attribute_null(VALUE self, VALUE recordIndex, VALUE fieldIndex)
{
  CHECK_ARGUMENT_FIXNUM(recordIndex);
  CHECK_ARGUMENT_FIXNUM(fieldIndex);

  dbf *db = unwrap(self);

  int result = DBFIsAttributeNULL(db->value(),
                                  FIX2INT(recordIndex),
                                  FIX2INT(fieldIndex));

  return INT2NUM(result);
}

VALUE dbf::close(VALUE self)
{
  dbf *db = unwrap(self);

  DBFClose(db->value());

  db->_handle = NULL;

  return Qnil;
}

VALUE dbf::get_field_count(VALUE self)
{
  dbf *db = unwrap(self);

  int fieldCount = DBFGetFieldCount(db->value());

  return INT2FIX(fieldCount);
}

VALUE dbf::get_record_count(VALUE self)
{
  dbf *db = unwrap(self);

  int recordCount = DBFGetRecordCount(db->value());

  return INT2FIX(recordCount);
}

VALUE dbf::get_field_index(VALUE self, VALUE fieldName)
{
  CHECK_ARGUMENT_STRING(fieldName);

  dbf *db = unwrap(self);

  int fieldIndex = DBFGetFieldIndex(db->value(), RSTRING_PTR(fieldName));

  return INT2FIX(fieldIndex);
}

VALUE dbf::get_field_info(VALUE self, VALUE fieldIndex)
{
  CHECK_ARGUMENT_FIXNUM(fieldIndex);

  dbf *db = unwrap(self);

  char fieldName[12];
  int fieldWidth = -1;
  int fieldDecimals = -1;

  DBFFieldType type = DBFGetFieldInfo(db->value(),
                                      FIX2INT(fieldIndex),
                                      fieldName,
                                      &fieldWidth,
                                      &fieldDecimals);

  VALUE hash = rb_hash_new();

  rb_hash_aset(hash, ID2SYM(rb_intern("name")), rb_str_new2(fieldName));
  rb_hash_aset(hash, ID2SYM(rb_intern("type")), INT2FIX((int)type));
  rb_hash_aset(hash, ID2SYM(rb_intern("width")), INT2FIX(fieldWidth));
  rb_hash_aset(hash, ID2SYM(rb_intern("decimals")), INT2FIX(fieldDecimals));

  return hash;
}

VALUE dbf::is_record_deleted(VALUE self, VALUE recordIndex)
{
  CHECK_ARGUMENT_FIXNUM(recordIndex);

  dbf *db = unwrap(self);

  int result = DBFIsRecordDeleted(db->value(), FIX2INT(recordIndex));

  return INT2FIX(result);
}

VALUE dbf::mark_record_deleted(VALUE self, VALUE recordIndex, VALUE isDeleted)
{
  CHECK_ARGUMENT_FIXNUM(recordIndex);

  dbf *db = unwrap(self);

  int result = DBFMarkRecordDeleted(db->value(), FIX2INT(recordIndex), FIX2INT(isDeleted));

  return INT2FIX(result);
}

VALUE dbf::get_native_field_type(VALUE self, VALUE fieldIndex)
{
  CHECK_ARGUMENT_FIXNUM(fieldIndex);

  dbf *db = unwrap(self);

  char result = DBFGetNativeFieldType(db->value(), FIX2INT(fieldIndex));

  char resultAsString[2] = { result, '\0' };

  return rb_str_new2(resultAsString);
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
  rb_define_method(dbf::_klass, "read_integer_attribute", SHP_METHOD(dbf::read_integer_attribute), 2);
  rb_define_method(dbf::_klass, "read_double_attribute", SHP_METHOD(dbf::read_double_attribute), 2);
  rb_define_method(dbf::_klass, "read_string_attribute", SHP_METHOD(dbf::read_string_attribute), 2);
  rb_define_method(dbf::_klass, "is_attribute_null", SHP_METHOD(dbf::is_attribute_null), 2);
  rb_define_method(dbf::_klass, "close", SHP_METHOD(dbf::close), 0);
  rb_define_method(dbf::_klass, "get_field_count", SHP_METHOD(dbf::get_field_count), 0);
  rb_define_method(dbf::_klass, "get_record_count", SHP_METHOD(dbf::get_record_count), 0);
  rb_define_method(dbf::_klass, "get_field_index", SHP_METHOD(dbf::get_field_index), 1);
  rb_define_method(dbf::_klass, "get_field_info", SHP_METHOD(dbf::get_field_info), 1);
  rb_define_method(dbf::_klass, "is_record_deleted", SHP_METHOD(dbf::is_record_deleted), 1);
  rb_define_method(dbf::_klass, "mark_record_deleted", SHP_METHOD(dbf::mark_record_deleted), 2);
  rb_define_method(dbf::_klass, "get_native_field_type", SHP_METHOD(dbf::get_native_field_type), 1);
}

}

