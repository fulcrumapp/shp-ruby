require 'mkmf'

shp_ruby_path = ENV['SHP_RUBY_PATH'] || '../../../..'

local_path = File.expand_path("#{ENV['SHP_RUBY_PATH']}/ext/shp/shapelib")

system("pushd #{local_path} && make clean && make && popd")

dir_config '', local_path, local_path

have_library 'shp' or raise 'libshp not found'

$libs = append_library $libs, 'shp'

create_makefile 'shp/shp'
