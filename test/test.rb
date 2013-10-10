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

(0..2000).each do |num|
  shape = shp.create_simple_object(1, 1, [rnd.rand(lon_range)], [rnd.rand(lat_range)], nil)
  shp.write_object(-1, shape)
  shape.compute_extents


  (0..100).each do |field|
    dbf.write_string_attribute(num, field, "Record #{num} Field #{field}")
  end
  # dbf.write_string_attribute(num, 0, "Record #{num}")
  # dbf.write_integer_attribute(num, 1, num)
  # dbf.write_double_attribute(num, 2, "#{num}.#{num}#{num}#{num}".to_f)
  # dbf.write_null_attribute(num, 3)
end

puts shp.get_info

dbf.close
shp.close

# write .prj file
prj_content = 'GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137,298.257223563]],PRIMEM["Greenwich",0],UNIT["Degree",0.017453292519943295]]'
File.open("testfile.prj", "wb") {|f| f.write(prj_content)}
