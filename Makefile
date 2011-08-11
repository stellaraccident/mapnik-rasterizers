# External configuration discovery
MAPNIK_CPPFLAGS := $(shell mapnik-config --cflags)
MAPNIK_LIBS := $(shell mapnik-config --libs)
MAPNIK_INPUT_PLUGIN_DIR := $(shell mapnik-config --input-plugins)

# Variable defaults
AGG_INCLUDE?=/usr/include/agg2
CPPFLAGS ?= -I$(AGG_INCLUDE)
CXXFLAGS ?=
CXXFLAGS_SHARED ?= -fPIC
LDFLAGS ?= 
CXX ?= g++

# Customizations
CPPFLAGS += $(MAPNIK_CPPFLAGS)

# Rules
%.os: %.cpp
	$(CXX) -c $(CXXFLAGS) $(CXXFLAGS_SHARED) $(CPPFLAGS) -MMD -MP -MF .$<.d $< -o $@

# Phony targets
all: heatmap.input

install: all
	cp $(wildcard *.input) $(MAPNIK_INPUT_PLUGIN_DIR)

clean:
	$(RM) $(wildcard *.os) $(wildcard .*.d) $(wildcard *.input)

.PHONY: all install clean

# Dependencies
include $(wildcard .*.d)

# Explicit Targets
heatmap.input: \
		heatmap_datasource.os chained_datasource.os \
		rasterizer_datasource.os density_mask.os
	$(CXX) -o $@ $(CXXFLAGS) $(CXXFLAGS_SHARED) -shared $+ \
		$(LDFLAGS) -Wl,--no-undefined $(MAPNIK_LIBS) 

	
	