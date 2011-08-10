/*
 * rasterizer_datasource.cpp
 *
 *  Created on: Aug 10, 2011
 *      Author: stella
 */
#include "rasterizer_datasource.hpp"

// Stdlib
#include <math.h>

// Mapnik
#include <mapnik/feature_factory.hpp>

// Local
#include "singleton_featureset.hpp"

rasterizer_datasource::rasterizer_datasource(const mapnik::parameters& params, bool bind):
	chained_datasource(params, bind)
{

}

rasterizer_datasource::~rasterizer_datasource()
{
}

int rasterizer_datasource::type() const
{
	return datasource::Raster;
}

mapnik::featureset_ptr rasterizer_datasource::features(const mapnik::query& q) const
{
	std::clog << "Rasterizer::features"
			<< ", resolution_type=(" << q.resolution().head << "," << q.resolution().tail.head << ")"
			<< ", scale_denominator=" << q.scale_denominator()
			<< ", bbox=[" << q.get_bbox().minx() << "," << q.get_bbox().miny() << "," << q.get_bbox().maxx() << "," << q.get_bbox().maxy() << "]"
			<< std::endl;

	double	resx=q.resolution().head,
			resy=q.resolution().tail.head,
			bbox_width=q.get_bbox().width(),
			bbox_height=q.get_bbox().height();

	int 	pixel_width=round(bbox_width * resx),
			pixel_height=round(bbox_height * resy);

	if (pixel_width<=0 || pixel_height<=0) {
		std::clog << "Illegal rasterizer display metrics" << std::endl;
		return mapnik::featureset_ptr();
	}

	std::clog << "Rasterizer generating image of size (" << pixel_width << "," << pixel_height << ")" << std::endl;

	// Setup for render
	mapnik::feature_ptr feature(mapnik::feature_factory::create(1));
	mapnik::image_data_32 image(pixel_width, pixel_height);
	rasterize_params params(q, image);

	rasterize(params);

	// Prepare and return the raster feature
	feature->set_raster(boost::make_shared<mapnik::raster>(q.get_bbox(), image));
	return boost::make_shared<singleton_featureset>(singleton_featureset(feature));
}

mapnik::featureset_ptr rasterizer_datasource::features_at_point(mapnik::coord2d const& pt) const
{
	// Point queries not supported
	return mapnik::featureset_ptr();
}
