#!/usr/bin/env ruby

$: << File.expand_path('../lib')
$: << File.expand_path('../ext')

require 'shp'

shp = SHP::Shapefile.create("testfile", 1)
dbf = SHP::DBF.create("testfile")

rnd = Random.new
lat_range = 26.0..28.0
lon_range = -82.0..-80.0

(0..100).each do |field|
  dbf.add_field("field_#{field}", 0, 254, 0)
end

dbf.add_field("integer_0", 1, 2^31, 0)
dbf.add_field("double_0", 2, 2^31, 10)
dbf.add_field("null_0", 1, 2^31, 0)

(0..2000).each do |num|
  shape = shp.create_simple_object(1, 1, [rnd.rand(lon_range)], [rnd.rand(lat_range)], nil)
  shp.write_object(-1, shape)
  shape.compute_extents
  shp.rewind_object(shape)

  (0..100).each do |field|
    dbf.write_string_attribute(num, field, "Record #{num} Field #{field}")
  end

  dbf.write_integer_attribute(num, dbf.get_field_index("integer_0"), 1337)
  dbf.write_double_attribute(num, dbf.get_field_index("double_0"), 1337.1337)
  dbf.write_null_attribute(num, dbf.get_field_index("null_0"))
end

puts shp.get_info
puts dbf.get_field_count == 101
puts dbf.get_record_count == 2001
puts dbf.get_field_index("field_1") == 1
puts dbf.get_field_info(0) == { name: "field_0", type: 0, width: 254, decimals: 0 }
puts dbf.read_integer_attribute(0, dbf.get_field_index("integer_0")) == 1337
puts dbf.read_double_attribute(0, dbf.get_field_index("double_0")) == 1337.1337

dbf.close
shp.close

# write .prj file
prj_content = 'GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137,298.257223563]],PRIMEM["Greenwich",0],UNIT["Degree",0.017453292519943295]]'
File.open("testfile.prj", "wb") {|f| f.write(prj_content)}
