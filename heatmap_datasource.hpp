/*
 * heatmap_datasource.hpp
 *
 *  Created on: Aug 8, 2011
 *      Author: stella
 */
#ifndef HEATMAP_DATASOURCE_HPP
#define HEATMAP_DATASOURCE_HPP

#include "rasterizer_datasource.hpp"

class heatmap_datasource: public rasterizer_datasource {
private:
	mapnik::layer_descriptor desc_;

	// No copy
	heatmap_datasource(const heatmap_datasource&);
	heatmap_datasource& operator=(const heatmap_datasource&);
protected:
	virtual void rasterize(rasterize_params& rp) const;

public:
	heatmap_datasource(const mapnik::parameters& params, bool bind=true);
	virtual ~heatmap_datasource();

	static std::string name();
    mapnik::layer_descriptor get_descriptor() const;
};



#endif /* HEATMAP_DATASOURCE_HPP_ */
