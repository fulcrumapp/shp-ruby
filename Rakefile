#!/usr/bin/env rake
require 'rubygems'
require 'bundler'
require 'bundler/gem_tasks'
require 'rake/testtask'
require 'rake'
require 'rake/extensiontask'
require 'rspec/core'
require 'rspec/core/rake_task'

Bundler.setup

Rake::ExtensionTask.new('shp') do |ext|
  ext.name = 'shp'
  ext.lib_dir = "lib/shp"
end

RSpec::Core::RakeTask.new(:spec) do |spec|
  Rake::Task['compile'].invoke
  spec.pattern = FileList['spec/**/*_spec.rb']
end

task :default => :spec
