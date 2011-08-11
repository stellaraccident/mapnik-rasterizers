mapnik-rasterizers
==================

This project is a set of mapnik raster input plugins intended to do
drawing of features where it is not appropriate to apply the painter's
algorithm with standard geometries.  Typically these types of jobs will
be done with high-end GIS applications, and this project is not trying
to displace that use.  However, there are some common map types that
are technically very simple to draw but embarrassingly difficult to trick
your standard web mapping server (such as mapnik) into producing.  Bringing
a full GIS solution into play, with the performance, mind-share and maintenance
overhead that entails may be overkill in some situations.

Installation
------------
This project is being developed against a mapnik2 trunk branch which may or
may not be up to date at any given time.  Therefore, changes in the trunk may
render this project uncompilable.

Assuming you have a "mapnik-config" command somewhere that make can find it, then
installation is as simple as

	make install
	
Or as two separate steps:

	make all
	sudo make install
	
Compilation also needs to be able to find AGG include files.  It defaults to /usr/include/agg2
which is where Ubuntu places the package.  You can override this (and even use the AGG from
a mapnik source install) by passing a parameter to make:

	make install AGG_INCLUDE=/path/to/headers
	
It probably only builds on Linux at the moment.  OSX should be a simple matter of
twiddling the flags to produce a shared library.

Heatmap
=======

This project was started in order to plot heatmaps.  Typically you will start with some
kind of a point datasource and want to display a colorized rendition of the densities.  There
are  alot of conceptual knobs that can be used to tweak and control the output, but not many
options have been un-hardcoded yet.

The basic idea is that for a given output area, a grid of floats is allocated.  For each sample
from, you update the value of the float at the appropriate position.  There are a few ways to
think of how to update the values based on what you are going for:

* set
* average
* add

If you are just trying to represent a density of binary things (either something is there or not)
then you probably want to use set or average.  If your data isn't very dense and you can handle
a little fudge in overlapping cases, set may work for you and you'll have a much better idea
up front about the range you are working with.  If you want the overlapping data to make things
more dense based on the overlap, you may use average.  If you are deriving the value based on some
count or weight attribute in the feature, you may want to use add (ie. a single point represents
some number of events, etc).

After all of the points are set on the surface, a Gaussian blur is performed at a determined radius.
You can think of this as "spreading out" the individual dots into blobs.  Blobs congeal together and
can intensify each other.  If we only did a Gaussian blur, then the average density over the expanded
area would add up to what it was previously at the point.  This is usually one of the selling points
of a Gaussian blur -- it preserves overall intensity in the areas it touches.  However, often we want to
exaggerate and purposely distort the data so that it is visible.  For this reason, the default behavior
is to relevel the image based on the level of peak reduction in intensity.

Finally, once the floating point values have been blurred appropriately, we have a grid of densities.
These are leveled out to a normalized range and cast to a byte value.  The byte value is used to look
up colors and alpha channels in two gradient lookup tables and the resulting color is set in the output
image.

### Configuring heatmap

To use the heatmap plugin, you will want to reference it in a datasource as type
"heatmap".  The plugin needs access to an underlying vector datasource it is trying to
render.  All parameters prefixed with a "src_" string are used to construct this vector
datasource.

Here is an example:

	<Style name="midheat">
		<Rule>
			<RasterSymbolizer/>
		</Rule>
	</Style>
	
	<Layer name="midheat" srs="+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +no_defs +over">
		<StyleName>midheat</StyleName>
		<Datasource>
			<Parameter name="type">heatmap</Parameter>
			<Parameter name="src_type">shape</Parameter>
			<Parameter name="src_file">/home/stella/gis/noxious_weed_wm.shp</Parameter>
		</Datasource>
	</Layer>

### Sample Maps

Portions of the following maps are Copyright OpenStreetMap and contributors, CC-BY-SA.  Statistical
data is provided by King County Washington http://www5.kingcounty.gov/sdc/Metadata.aspx?Layer=park_label

#### Park Label Density

This map image was produced by adding a heatmap layer which renders off of the [King County
park label point datasource](http://www5.kingcounty.gov/sdc/Metadata.aspx?Layer=park_label).


#### Noxious Weed Density

This map image was produced by adding a heatmap layer which renders off of the [King County
noxious weed survey sitings datasource](http://www5.kingcounty.gov/sdc/Metadata.aspx?Layer=noxious_weed).


