/*
 * chained_datasource.hpp
 *
 *  Created on: Aug 8, 2011
 *      Author: stella
 */

#ifndef CHAINED_DATASOURCE_HPP
#define CHAINED_DATASOURCE_HPP

// mapnik
#include <mapnik/feature.hpp>
#include <mapnik/datasource.hpp>

/**
 * chained_datasource
 * Base class for constructing datasources that depend on other
 * datasources.  The source datasource is constructed using
 * parameters prefixed with "src_".
 */
class chained_datasource: public mapnik::datasource {
private:
	mapnik::parameters source_params_;
	mutable mapnik::datasource_ptr source_;

public:
	chained_datasource(const mapnik::parameters& params, bool bind=true);
	virtual ~chained_datasource();

	mapnik::box2d<double> envelope() const;
    void bind() const;

protected:
    mapnik::datasource_ptr source() const {
    	return source_;
    }
};

#endif /* CHAINED_DATASOURCE_HPP_ */
