require 'mkmf'

shapelib_path = File.expand_path("#{File.dirname(__FILE__)}/shapelib")

system "cd #{shapelib_path} && make clean && make && cd #{Dir.pwd}"

dir_config '', shapelib_path, shapelib_path

have_library 'shp' or raise 'libshp not found'

$libs = append_library $libs, 'shp'

create_makefile 'shp/shp'
