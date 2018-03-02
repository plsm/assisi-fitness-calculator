#ifndef __HISTOGRAM__
#define __HISTOGRAM__

#include <stdio.h>
#include <vector>
#include <string>

#include "image.hpp"

class Histogram:
      public std::vector<double>
{
public:
	Histogram ();
	/**
	 * @brief read Read an histogram from the given file.  The data format is CSV.
	 * @param file
	 */
	void read (FILE *file);
	/**
	 * @brief write Write a histogram to the given file.  The data format is CSV.
	 * @param file
	 */
	void write (FILE *file) const;
	/**
	 * Return the most common colour in this histogram.
	 */
	int most_common_colour () const;


	int number_different_pixels (unsigned int same_colour_level) const
	{
		int result = 0;
		for (unsigned int i = same_colour_level; i < NUMBER_COLOUR_LEVELS; i++)
			result += (*this) [i];
		return result;
	}

};

typedef std::vector<Histogram> VectorHistograms;

void write_vector_histograms (const std::string &filename, const VectorHistograms *vh);
VectorHistograms *read_vector_histograms (const std::string &filename, size_t size);

#endif
