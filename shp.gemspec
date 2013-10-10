# -*- encoding: utf-8 -*-
require File.expand_path('../lib/shp/version', __FILE__)

Gem::Specification.new do |gem|
  gem.authors       = ["Zac McCormick"]
  gem.email         = ["zac.mccormick@gmail.com"]
  gem.description   = %q{ESRI Shapefile bindings for ruby using shapelib}
  gem.summary       = %q{ESRI Shapefile bindings for ruby using shapelib. Currently contains native extensions for ShapeLib 1.3}
  gem.homepage      = "https://github.com/zhm/shp-ruby"

  gem.files         = `git ls-files`.split($\)
  gem.extensions    = ['ext/shp/extconf.rb']
  gem.executables   = gem.files.grep(%r{^bin/}).map{ |f| File.basename(f) }
  gem.test_files    = gem.files.grep(%r{^(test|spec|features)/})
  gem.name          = "shp"
  gem.require_paths = ['lib', 'ext']
  gem.version       = SHP::VERSION

  # gem.add_development_dependency 'rake',          ['>= 0']
  # gem.add_development_dependency 'rake-compiler', ['>= 0']
  # gem.add_development_dependency 'rspec',         ['>= 0']
end
