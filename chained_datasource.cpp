/*
 * chained_datasource.cpp
 *
 *  Created on: Aug 8, 2011
 *      Author: stella
 */
#include "chained_datasource.hpp"

// Mapnik
#include <mapnik/datasource_cache.hpp>

// Boost
#include <boost/algorithm/string/predicate.hpp>

using mapnik::parameters;
using mapnik::datasource_cache;

chained_datasource::chained_datasource(const parameters& params, bool _bind) :
	datasource(params)
{
	// Iterate over params and populate m_source_params
	// from all parameters that start with "src_"
	for (parameters::const_iterator iter=params.begin(); iter!=params.end(); iter++) {
		const std::string &name(iter->first);
		if (!boost::starts_with(name, "src_")) continue;

		const std::string sub_name(name.substr(4));
		source_params_[sub_name]=iter->second;
	}

	if (_bind) {
		bind();
	}
}

chained_datasource::~chained_datasource()
{
}

mapnik::box2d<double> chained_datasource::envelope() const
{
	if (source_) {
		return source_->envelope();
	} else {
		return mapnik::box2d<double>(-180,-90,180,90);	// TODO: Remove/warn
	}
}

void chained_datasource::bind() const
{
	return;
	if (is_bound_) return;

	source_=datasource_cache::create(source_params_, true);
	is_bound_=true;
}


