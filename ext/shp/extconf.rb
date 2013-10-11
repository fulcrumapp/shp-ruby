require 'mkmf'

shp_ruby_path = ENV['SHP_RUBY_PATH'] || '../../../..'

local_path = File.expand_path("#{ENV['SHP_RUBY_PATH']}/ext/shp/shapelib")

current_dir = Dir.pwd

system("cd #{local_path} && make clean && make && cd #{current_dir}")

dir_config '', local_path, local_path

have_library 'shp' or raise 'libshp not found'

$libs = append_library $libs, 'shp'

create_makefile 'shp/shp'
