/*
 * rasterizer_datasource.hpp
 *
 *  Created on: Aug 8, 2011
 *      Author: stella
 */

#ifndef RASTERIZER_DATASOURCE_HPP
#define RASTERIZER_DATASOURCE_HPP

#include "chained_datasource.hpp"

// Forward defines
class rasterizer_accumulator;
class rasterizer_datasource;

/**
 * Accumulates raster results.  This is constructed and dispatched internally
 * by the rasterizer_datasource and calls back to virtual methods within
 * the datasource to do its work.
 */
class rasterize_params {
	const mapnik::query& query_;
	mapnik::image_data_32& image_;
public:
	rasterize_params(const mapnik::query& query, mapnik::image_data_32& image):
		query_(query),
		image_(image)
	{
	}

	mapnik::image_data_32& image() {
		return image_;
	}

	const mapnik::query& query() {
		return query_;
	}
};

/**
 * rasterizer_datasource
 * Base class for constructing datasources that process features from
 * another datasource and plot them on an image surface that is returned
 * as a raster.
 */
class rasterizer_datasource: public chained_datasource {

protected:
	virtual void rasterize(rasterize_params& rp) const = 0;

public:
	rasterizer_datasource(const mapnik::parameters& params, bool bind=true);
	virtual ~rasterizer_datasource();

	int type() const;
    mapnik::featureset_ptr features(const mapnik::query& q) const;
    mapnik::featureset_ptr features_at_point(mapnik::coord2d const& pt) const;
};

#endif
