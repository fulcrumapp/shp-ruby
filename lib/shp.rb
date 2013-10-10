require 'shp/version'
require 'shp/shp'

module SHP
  SHPT_NULL        = 0
  SHPT_POINT       = 1
  SHPT_ARC         = 3
  SHPT_POLYGON     = 5
  SHPT_MULTIPOINT  = 8
  SHPT_POINTZ      = 11
  SHPT_ARCZ        = 13
  SHPT_POLYGONZ    = 15
  SHPT_MULTIPOINTZ = 18
  SHPT_POINTM      = 21
  SHPT_ARCM        = 23
  SHPT_POLYGONM    = 25
  SHPT_MULTIPOINTM = 28
  SHPT_MULTIPATCH  = 31
end
