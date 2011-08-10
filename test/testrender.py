#!/usr/bin/env python

from mapnik2 import *
import os.path

# Register datasources
dev_path=os.path.abspath(os.path.dirname(__file__) + '/..')
print "Dev plugin path=" + dev_path
DatasourceCache.register_datasources(dev_path)
names=DatasourceCache.plugin_names()
print "Registered input plugins:"
for i in range(len(names)):
	name=names[i]
	print "\t%s" % name
    
    
m=Map(640, 480, '+proj=latlong +dataum=WGS84')
m.background=Color('white')
m.buffer_size=0
m.aspect_fix_mode=aspect_fix_mode.ADJUST_BBOX_HEIGHT

# Datasource
datasource=Datasource(
	type='heatmap',
	src_type='shape',
	src_file='/home/stella/gis/park_label_ll.shp'
	)

# Layer
lyr=Layer('heatmap')
lyr.datasource=datasource
lyr.srs='+proj=latlong +dataum=WGS84'
lyr.extent=Box2d(-180, -90, 180, 90)
lyr.styles.append('blit')
m.layers.append(lyr)

# Style
s=Style()
r=Rule()
rs=RasterSymbolizer()
r.symbols.extend([rs])
s.rules.append(r)
m.append_style('blit', s)

#env=Box2d(-122.562276, 47.116973, -122.233373, 47.300291)
env=Box2d(-122.526560, 47.144090, -121.080983, 47.775753)
#env=Box2d(-180, -90, 180, 90)
m.zoom_to_box(env)
print "Rendering map for " + repr(m.envelope())
print "Buffered envelope " + repr(m.buffered_envelope())
render_to_file(m, 'testheatmap.png')
