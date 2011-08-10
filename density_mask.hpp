/*
 * density_mask.hpp
 *
 *  Created on: Aug 10, 2011
 *      Author: stella
 */

#ifndef DENSITY_MASK_HPP
#define DENSITY_MASK_HPP

#include <algorithm>

#include "agg_basics.h"
#include "agg_pixfmt_gray.h"

/**
 * Contains a rectangular grid of floating point samples and provides
 * operations for smoothing and transferring them to a density_mask as
 * pixel values.
 */
class density_mask {
	float* m_buffer;
	unsigned m_width;
	unsigned m_height;

	float m_minlevel;
	float m_maxlevel;

	density_mask(const density_mask& other);
	density_mask& operator=(const density_mask& other);

public:
	density_mask(unsigned width, unsigned height):
		m_width(width),
		m_height(height),
		m_minlevel(0),
		m_maxlevel(1)
	{
		m_buffer=new float[width*height];
	}
	~density_mask() {
		delete[] m_buffer;
	}

	// Sample operations
	void set_sample(int x, int y, float value) {
		if (x<0 || y<0 || x>=(int)m_width || y>=(int)m_height) return;

		m_buffer[y*m_width+x]=value;
	}

	float get_sample(int x, int y) {
		if (x<0 || y<0 || x>=(int)m_width || y>=(int)m_height) return 0;

		return m_buffer[y*m_width+x];
	}

	void set_random_sample(bool use_random_value) {
		// Note: this is just for testing and plays a little fast and loose
		// with ranges (safe, just not accurate)
		int x=(int)(m_width * ((float)rand() / ((float)RAND_MAX)));
		int y=(int)(m_height * ((float)rand() / ((float)RAND_MAX)));
		float value=use_random_value ? (float)rand() / (float)RAND_MAX : 1.0;

		set_sample(x, y, value);
	}

	void set_random_samples(int count, bool use_random_value) {
		for (int i=0; i<count; i++) {
			set_random_sample(use_random_value);
		}
	}

	agg::int8u normalize_sample(float sample) {
		float norm=(sample - m_minlevel) / m_maxlevel;
		int v=255 * norm;
		if (v<0) v=0;
		else if (v>255) v=255;
		return v;
	}

	/**
	 * Calculate the current maximum range of values
	 */
	float range_max();

	/**
	 * Multiply all values by the given factor
	 */
	void multiply(float factor);

	/**
	 * Perform a gaussian blend of the given radius, optionally
	 * adjusting the intensity to be of the same ratio as the original.
	 */
	void gaussian_blend(int radius, bool correct_intensity);

	template<class pixf_t,class color_lut_t,class alpha_lut_t>
	void colorize(pixf_t pixf, color_lut_t color_lut, alpha_lut_t alpha_lut)
	{
		unsigned width=std::min(m_width,pixf.width());
		unsigned height=std::min(m_height,pixf.height());

		for (unsigned y=0; y<height; y++) {
			for (unsigned x=0; x<width; x++) {
				float sample=m_buffer[y*m_width+x];
				agg::int8u maskval=normalize_sample(sample);
				agg::rgba8 color=color_lut[maskval];
				agg::gray8 alpha=alpha_lut[maskval];
				color.a=alpha.v;
				pixf.copy_pixel(x, y, color);
			}
		}
	}
};



#endif /* DENSITY_MASK_HPP_ */
