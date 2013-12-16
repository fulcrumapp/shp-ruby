require 'shp'

describe "SHP" do
  before(:all) do
    @shp = SHP::Shapefile.create("testfile", 1)
    @dbf = SHP::DBF.create("testfile")

    rnd = Random.new
    lat_range = 26.0..28.0
    lon_range = -82.0..-80.0

    (0..100).each do |field|
      @dbf.add_field("field_#{field}", 0, 254, 0)
    end

    @dbf.add_field("integer_0", 1, 2^31, 0)
    @dbf.add_field("double_0", 2, 2^31, 10)
    @dbf.add_field("null_0", 1, 2^31, 0)

    (0..2000).each do |num|
      shape = SHP::Shapefile.create_simple_object(1, 1, [rnd.rand(lon_range)], [rnd.rand(lat_range)], nil)
      @shp.write_object(-1, shape)
      shape.compute_extents
      @shp.rewind_object(shape)

      (0..100).each do |field|
        @dbf.write_string_attribute(num, field, "Record #{num} Field #{field}")
      end

      @dbf.write_integer_attribute(num, @dbf.get_field_index("integer_0"), 1337)
      @dbf.write_double_attribute(num, @dbf.get_field_index("double_0"), 1337.1337)
      @dbf.write_null_attribute(num, @dbf.get_field_index("null_0"))
    end
  end

  after(:all) do
    @dbf.close
    @shp.close

    # write .prj file
    prj_content = 'GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137,298.257223563]],PRIMEM["Greenwich",0],UNIT["Degree",0.017453292519943295]]'

    File.open("testfile.prj", "wb") {|f| f.write(prj_content)}
  end

  it 'open should open a shapefile for reading' do
    shp = SHP::Shapefile.open('testfile', 'rb')
    shp.should_not be_nil
  end

  it 'get_info to return the shapefile info' do
    info = @shp.get_info
    info[:number_of_entities].should eq(2001)
    info[:shape_type].should eq(1)
  end

  it 'get_field_count to return the field count' do
    @dbf.get_field_count.should eq(104)
  end

  it 'get_record_count to return the record count' do
    @dbf.get_record_count.should eq(2001)
  end

  it 'get_field_index to return the field index' do
    @dbf.get_field_index("field_1").should eq(1)
  end

  it 'get_field_info to return the field info' do
    @dbf.get_field_info(0).should eq({ name: "field_0", type: 0, width: 254, decimals: 0 })
  end

  it 'read_integer_attribute to read the correct integer value' do
    @dbf.read_integer_attribute(0, @dbf.get_field_index("integer_0")).should eq(1337)
  end

  it 'read_double_attribute to read the correct double value' do
    @dbf.read_double_attribute(0, @dbf.get_field_index("double_0")).should eq(1337.1337)
  end

  it 'read_string_attribute to read the correct string value' do
    @dbf.read_string_attribute(0, @dbf.get_field_index("field_0")).should eq('Record 0 Field 0')
  end

  it 'is_attribute_null to check if an attribute is NULL' do
    @dbf.is_attribute_null(0, @dbf.get_field_index("null_0")).should eq(1)
  end

  it 'is_record_deleted to return 0 when a record has not been deleted' do
    @dbf.is_record_deleted(0).should eq(0)
  end

  it 'mark_record_deleted to mark a record as deleted' do
    @dbf.mark_record_deleted(0, 1).should eq(1)
  end

  it 'is_record_deleted to return 1 when a record has been deleted' do
    @dbf.is_record_deleted(0).should eq(1)
  end

  it 'get_native_field_type on integer to be correct' do
    @dbf.get_native_field_type(@dbf.get_field_index("integer_0")).should eq(SHP::DBF::FT_NATIVE_TYPE_INTEGER)
  end

  it 'get_native_field_type on double to be correct' do
    @dbf.get_native_field_type(@dbf.get_field_index("double_0")).should eq(SHP::DBF::FT_NATIVE_TYPE_DOUBLE)
  end

  it 'get_native_field_type on string to be correct' do
    @dbf.get_native_field_type(@dbf.get_field_index("field_0")).should eq(SHP::DBF::FT_NATIVE_TYPE_STRING)
  end

  it 'should raise an error when destroy is called twice' do
    shape = SHP::Shapefile.create_simple_object(1, 1, [-82.1], [-27.2], nil)
    lambda { shape.destroy }.should_not raise_error
    lambda { shape.destroy }.should raise_error
  end

  it 'should raise an error when calling a method after it has been destroyed' do
    shape = SHP::Shapefile.create_simple_object(1, 1, [-82.1], [-27.2], nil)
    lambda { shape.destroy }.should_not raise_error
    lambda { shape.compute_extents }.should raise_error
  end

  it 'should raise an error when calling a method after it has been closed' do
    file = SHP::Shapefile.create("testfile2", 1)
    lambda { file.close }.should_not raise_error
    lambda { file.get_info }.should raise_error
  end

  context 'shp' do
    before(:each) do
      sni_office = [ [ -82.72932529449463, 27.93789618055838 ],
                     [ -82.72932529449463, 27.93768765436987 ],
                     [ -82.72909998893738, 27.93767817589719 ],
                     [ -82.72911071777344, 27.93719003343022 ],
                     [ -82.72869229316710, 27.93717581565543 ],
                     [ -82.72868156433105, 27.93741277832466 ],
                     [ -82.72886931896210, 27.93741751757274 ],
                     [ -82.72886931896210, 27.93788670210399 ],
                     [ -82.72932529449463, 27.93789618055838 ] ]

      x_values = sni_office.map { |v| v[0] }
      y_values = sni_office.map { |v| v[1] }

      @shp = SHP::Shapefile.create('testfile_polygons', 5)
      @shape = SHP::Shapefile.create_simple_object(5, x_values.count, x_values, y_values, nil)
      @shp.write_object(-1, @shape)
      @obj = @shp.read_object(0)
    end

    it 'should return the shape type' do
      @obj.get_shape_type.should eq(5)
    end

    it 'should return the shape id' do
      @obj.get_shape_id.should eq(0)
    end

    it 'should return the shape part count' do
      @obj.get_shape_parts.should eq(1)
    end

    it 'should return the shape part start offsets' do
      @obj.get_shape_part_starts.should eq([0])
    end

    it 'should return the shape part types' do
      @obj.get_shape_part_types.should eq([5]) # [SHPP_RING]
    end

    it 'should return the vertex count' do
      @obj.get_vertex_count.should eq(9)
    end

    it 'should return the x values' do
      @obj.get_x.should eq([-82.72932529449463, -82.72932529449463, -82.72909998893738, -82.72911071777344, -82.7286922931671, -82.72868156433105, -82.7288693189621, -82.7288693189621, -82.72932529449463])
    end

    it 'should return the y values' do
      @obj.get_y.should eq([27.93789618055838, 27.93768765436987, 27.93767817589719, 27.93719003343022, 27.93717581565543, 27.93741277832466, 27.93741751757274, 27.93788670210399, 27.93789618055838])
    end
  end
end
