
#include "shapefile.hpp"
#include "shape_object.hpp"

namespace shp {

VALUE shapefile::_klass = Qnil;

VALUE shapefile::klass() {
  return shapefile::_klass;
}

shapefile::shapefile(SHPHandle handle)
  : _handle(handle)
{
}

shapefile::~shapefile() {
  if (_handle) {
    SHPClose(_handle);
    _handle = NULL;
  }
}

VALUE shapefile::open(VALUE klass, VALUE filename, VALUE accessType)
{
  CHECK_ARGUMENT_STRING(filename);
  CHECK_ARGUMENT_STRING(accessType);

  SHPHandle handle = SHPOpen(RSTRING_PTR(filename), RSTRING_PTR(accessType));

  if (handle == NULL) {
    SHP_FATAL("Failed to open shapefile.");
    return Qnil;
  }

  shapefile *shp = new shapefile(handle);

  return shp->wrapped();
}

VALUE shapefile::create(VALUE klass, VALUE filename, VALUE shapeType)
{
  CHECK_ARGUMENT_STRING(filename);
  CHECK_ARGUMENT_FIXNUM(shapeType);

  SHPHandle handle = SHPCreate(RSTRING_PTR(filename), FIX2INT(shapeType));

  if (handle == NULL) {
    SHP_FATAL("Failed to create shapefile.");
    return Qnil;
  }

  shapefile *shp = new shapefile(handle);

  return shp->wrapped();
}

VALUE shapefile::create_simple_object(VALUE klass, VALUE shapeType, VALUE numberOfVertices, VALUE arrayOfX, VALUE arrayOfY, VALUE arrayOfZ)
{
  CHECK_ARGUMENT_FIXNUM(shapeType);
  CHECK_ARGUMENT_FIXNUM(numberOfVertices);
  CHECK_ARGUMENT_ARRAY(arrayOfX);
  CHECK_ARGUMENT_ARRAY(arrayOfY);

  double *xVertices = new double[FIX2INT(numberOfVertices)];
  double *yVertices = new double[FIX2INT(numberOfVertices)];
  double *zVertices = NULL;

  if (!NIL_P(arrayOfZ)) {
    CHECK_ARGUMENT_ARRAY(arrayOfZ);
    zVertices = new double[FIX2INT(numberOfVertices)];
  }

  for (int i = 0; i < FIX2INT(numberOfVertices); ++i) {
    xVertices[i] = NUM2DBL(rb_ary_entry(arrayOfX, i));
    yVertices[i] = NUM2DBL(rb_ary_entry(arrayOfY, i));

    if (zVertices) {
      zVertices[i] = NUM2DBL(rb_ary_entry(arrayOfZ, i));
    }
  }

  SHPObject *object = SHPCreateSimpleObject(FIX2INT(shapeType),
                                            FIX2INT(numberOfVertices),
                                            xVertices,
                                            yVertices,
                                            zVertices);

  delete [] xVertices;
  delete [] yVertices;

  if (zVertices) {
    delete [] zVertices;
  }

  if (object == NULL) {
    SHP_FATAL("Failed to create shape object.");
    return Qnil;
  }

  shape_object *shape = new shape_object(object);

  return shape->wrapped();
}


VALUE shapefile::create_object(VALUE klass, VALUE shapeType, VALUE shapeIndex, VALUE numberOfParts,
                               VALUE arrayOfPartStarts, VALUE arrayOfPartTypes, VALUE numberOfVertices,
                               VALUE arrayOfX, VALUE arrayOfY, VALUE arrayOfZ, VALUE arrayOfM)
{
  CHECK_ARGUMENT_FIXNUM(shapeType);
  CHECK_ARGUMENT_FIXNUM(shapeIndex);
  CHECK_ARGUMENT_FIXNUM(numberOfParts);
  CHECK_ARGUMENT_FIXNUM(numberOfVertices);
  CHECK_ARGUMENT_ARRAY(arrayOfX);
  CHECK_ARGUMENT_ARRAY(arrayOfY);

  double *xVertices = new double[FIX2INT(numberOfVertices)];
  double *yVertices = new double[FIX2INT(numberOfVertices)];
  double *zVertices = NULL;
  double *mVertices = NULL;

  if (!NIL_P(arrayOfZ)) {
    CHECK_ARGUMENT_ARRAY(arrayOfZ);
    zVertices = new double[FIX2INT(numberOfVertices)];
  }

  if (!NIL_P(arrayOfM)) {
    CHECK_ARGUMENT_ARRAY(arrayOfM);
    mVertices = new double[FIX2INT(numberOfVertices)];
  }

  for (int i = 0; i < FIX2INT(numberOfVertices); ++i) {
    xVertices[i] = NUM2DBL(rb_ary_entry(arrayOfX, i));
    yVertices[i] = NUM2DBL(rb_ary_entry(arrayOfY, i));

    if (zVertices) {
      zVertices[i] = NUM2DBL(rb_ary_entry(arrayOfZ, i));
    }

    if (mVertices) {
      mVertices[i] = NUM2DBL(rb_ary_entry(arrayOfM, i));
    }
  }

  int *nativeArrayOfPartStarts = NULL;
  int *nativeArrayOfPartTypes  = NULL;

  if (!NIL_P(arrayOfPartStarts)) {
    CHECK_ARGUMENT_ARRAY(arrayOfPartStarts);
    nativeArrayOfPartStarts = new int[FIX2INT(numberOfParts)];

    for (int i = 0; i < FIX2INT(numberOfParts); ++i) {
      nativeArrayOfPartStarts[i] = FIX2INT(rb_ary_entry(arrayOfPartStarts, i));
    }
  }

  if (!NIL_P(arrayOfPartTypes)) {
    CHECK_ARGUMENT_ARRAY(arrayOfPartTypes);
    nativeArrayOfPartTypes = new int[FIX2INT(numberOfParts)];

    for (int i = 0; i < FIX2INT(numberOfParts); ++i) {
      nativeArrayOfPartTypes[i] = FIX2INT(rb_ary_entry(arrayOfPartTypes, i));
    }
  }

  SHPObject *object = SHPCreateObject(FIX2INT(shapeType),
                                      FIX2INT(shapeIndex),
                                      FIX2INT(numberOfParts),
                                      nativeArrayOfPartStarts,
                                      nativeArrayOfPartTypes,
                                      FIX2INT(numberOfVertices),
                                      xVertices,
                                      yVertices,
                                      zVertices,
                                      mVertices);

  delete [] xVertices;
  delete [] yVertices;

  if (zVertices) {
    delete [] zVertices;
  }

  if (mVertices) {
    delete [] mVertices;
  }

  if (nativeArrayOfPartStarts) {
    delete [] nativeArrayOfPartStarts;
  }

  if (nativeArrayOfPartTypes) {
    delete [] nativeArrayOfPartTypes;
  }

  if (object == NULL) {
    SHP_FATAL("Failed to create shape object.");
    return Qnil;
  }

  shape_object *shape = new shape_object(object);

  return shape->wrapped();
}


VALUE shapefile::rewind_object(VALUE self, VALUE shapeObject) {
  shapefile *file = unwrap(self);
  shape_object *shape = shape_object::unwrap(shapeObject);

  CHECK_VALID_HANDLE(file->value());
  CHECK_VALID_HANDLE(shape->value());

  if (shape == NULL) {
    SHP_FATAL("You must specify a valid shape.");
    return Qnil;
  }

  int result = SHPRewindObject(file->value(), shape->value());

  return INT2FIX(result);
}

VALUE shapefile::get_info(VALUE self)
{
  shapefile *file = unwrap(self);

  CHECK_VALID_HANDLE(file->value());

  int numberOfEntities = 0;
  int shapeType = 0;
  double minBounds[4];
  double maxBounds[4];

  SHPGetInfo(file->value(),
             &numberOfEntities,
             &shapeType,
             minBounds,
             maxBounds);

  VALUE hash = rb_hash_new();

  VALUE minBoundsAry = rb_ary_new();
  VALUE maxBoundsAry = rb_ary_new();

  rb_ary_push(minBoundsAry, rb_float_new(minBounds[0]));
  rb_ary_push(minBoundsAry, rb_float_new(minBounds[1]));
  rb_ary_push(minBoundsAry, rb_float_new(minBounds[2]));
  rb_ary_push(minBoundsAry, rb_float_new(minBounds[3]));

  rb_ary_push(maxBoundsAry, rb_float_new(maxBounds[0]));
  rb_ary_push(maxBoundsAry, rb_float_new(maxBounds[1]));
  rb_ary_push(maxBoundsAry, rb_float_new(maxBounds[2]));
  rb_ary_push(maxBoundsAry, rb_float_new(maxBounds[3]));

  rb_hash_aset(hash, ID2SYM(rb_intern("number_of_entities")), INT2FIX(numberOfEntities));
  rb_hash_aset(hash, ID2SYM(rb_intern("shape_type")), INT2FIX(shapeType));
  rb_hash_aset(hash, ID2SYM(rb_intern("min_bounds")), minBoundsAry);
  rb_hash_aset(hash, ID2SYM(rb_intern("max_bounds")), maxBoundsAry);

  return hash;
}

VALUE shapefile::read_object(VALUE self, VALUE shapeIndex)
{
  CHECK_ARGUMENT_FIXNUM(shapeIndex);

  shapefile *file = unwrap(self);

  CHECK_VALID_HANDLE(file->value());

  SHPObject *object = SHPReadObject(file->value(), FIX2INT(shapeIndex));

  if (object == NULL) {
    SHP_FATAL("Unable to read shape object.");
    return Qnil;
  }

  shape_object *shape = new shape_object(object);

  return shape->wrapped();
}

VALUE shapefile::write_object(VALUE self, VALUE shapeIndex, VALUE shapeObject)
{
  CHECK_ARGUMENT_FIXNUM(shapeIndex);

  shapefile *file = unwrap(self);
  shape_object *object = shape_object::unwrap(shapeObject);

  CHECK_VALID_HANDLE(file->value());
  CHECK_VALID_HANDLE(object->value());

  int result = SHPWriteObject(file->value(), FIX2INT(shapeIndex), object->value());

  return INT2FIX(result);
}

VALUE shapefile::close(VALUE self)
{
  shapefile *file = unwrap(self);

  CHECK_VALID_HANDLE(file->value());

  SHPClose(file->value());

  file->_handle = 0;

  return Qnil;
}

void shapefile::define(VALUE module)
{
  shapefile::_klass = rb_define_class_under(module, "Shapefile", rb_cObject);
  base::define(shapefile::_klass, false);
  rb_define_singleton_method(shapefile::_klass, "open", SHP_METHOD(shapefile::open), 2);
  rb_define_singleton_method(shapefile::_klass, "create", SHP_METHOD(shapefile::create), 2);
  rb_define_singleton_method(shapefile::_klass, "create_simple_object", SHP_METHOD(shapefile::create_simple_object), 5);
  rb_define_singleton_method(shapefile::_klass, "create_object", SHP_METHOD(shapefile::create_object), 10);
  rb_define_method(shapefile::_klass, "write_object", SHP_METHOD(shapefile::write_object), 2);
  rb_define_method(shapefile::_klass, "close", SHP_METHOD(shapefile::close), 0);
  rb_define_method(shapefile::_klass, "get_info", SHP_METHOD(shapefile::get_info), 0);
  rb_define_method(shapefile::_klass, "read_object", SHP_METHOD(shapefile::read_object), 1);
  rb_define_method(shapefile::_klass, "rewind_object", SHP_METHOD(shapefile::rewind_object), 1);
}

}

