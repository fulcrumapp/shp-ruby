require 'mkmf'

system('cd shapelib && make clean && make && cd ..')

dir_config '', 'shapelib', 'shapelib'

have_library 'shp' or raise 'libshp not found'

$libs = append_library $libs, 'shp'

create_makefile 'shp/shp'
