/*
 * singleton_featureset.hpp
 *
 *  Created on: Aug 10, 2011
 *      Author: stella
 */

#ifndef SINGLETON_FEATURESET_HPP
#define SINGLETON_FEATURESET_HPP

#include <mapnik/datasource.hpp>

class singleton_featureset: public mapnik::Featureset
{
	mapnik::feature_ptr feature_;

public:
	singleton_featureset(mapnik::feature_ptr feature):
		feature_(feature)
	{
	}

	virtual ~singleton_featureset()
	{
	}

	mapnik::feature_ptr next()
	{
		mapnik::feature_ptr result(feature_);
		feature_.reset();
		return result;
	}
};


#endif /* SINGLETON_FEATURESET_HPP_ */
