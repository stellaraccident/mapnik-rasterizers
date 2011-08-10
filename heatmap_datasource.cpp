/*
 * heatmap_datasource.cpp
 *
 *  Created on: Aug 8, 2011
 *      Author: stella
 */
#include "heatmap_datasource.hpp"
#include "density_mask.hpp"

#include "agg_gradient_lut.h"
#include "agg_rendering_buffer.h"
#include "agg_pixfmt_rgba.h"

using mapnik::datasource;
using mapnik::parameters;

DATASOURCE_PLUGIN(heatmap_datasource);

heatmap_datasource::heatmap_datasource(const mapnik::parameters& params, bool bind) :
		rasterizer_datasource(params, bind),
		desc_("heatmap", "utf-8")
{

}

heatmap_datasource::~heatmap_datasource()
{

}

std::string heatmap_datasource::name()
{
	return "heatmap";
}

mapnik::layer_descriptor heatmap_datasource::get_descriptor() const
{
	return desc_;
}

void heatmap_datasource::rasterize(rasterize_params& rp) const
{
	density_mask mask(rp.image().width(), rp.image().height());
	mask.set_random_samples(250, true);
	mask.gaussian_blend(20, true);

	// Color gradient
	agg::gradient_lut<agg::color_interpolator<agg::rgba8> > color_lut;
	color_lut.remove_all();
	color_lut.add_color(0, agg::rgba8(0,0,128,0));
	color_lut.add_color(0.45, agg::rgba8(0,0,255,0));
	color_lut.add_color(0.55, agg::rgba8(0,255,255,0));
	color_lut.add_color(0.65, agg::rgba8(0,255,0,0));
	color_lut.add_color(0.95, agg::rgba8(139,139,0,0));
	color_lut.add_color(1.0, agg::rgba8(255,0,0,0));
	color_lut.build_lut();

	agg::gradient_lut<agg::color_interpolator<agg::gray8> > alpha_lut;
	alpha_lut.remove_all();
	alpha_lut.add_color(0, agg::gray8(0));
	alpha_lut.add_color(0.1, agg::gray8(150));
	alpha_lut.add_color(1.0, agg::gray8(255));
	alpha_lut.build_lut();

	// Setup agg rendering on top of output image buffer
	agg::rendering_buffer rbuf(rp.image().getBytes(),
			rp.image().width(),
			rp.image().height(),
			rp.image().width() * 4);
	agg::pixfmt_rgba32 pixf(rbuf);

	mask.colorize(pixf, color_lut, alpha_lut);
}
