# shp-ruby

Native bindings for Shapelib. This gem includes an embedded copy of Shapelib so it doesn't require any
system libraries to be installed and can be installed with just gem/bundler.

This gem is intentionally low level and aims to be a 1:1 mapping of the native C API. It does not
add any Ruby sugar on top of Shapelib. A more Ruby-like API can be implemented *in ruby* on top of this core API.

Docs for the native API:

[SHP](http://shapelib.maptools.org/shp_api.html)
[DBF](http://shapelib.maptools.org/dbf_api.html)

## Installation

Add this to your `Gemfile`

```rb
gem 'shp'
```

- OR install manually -

```sh
gem install shp
```

## License

BSD
