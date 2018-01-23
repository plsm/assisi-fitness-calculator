#ifndef ANALYSISPARAMETERS_HPP
#define ANALYSISPARAMETERS_HPP

#include <string>
#include <boost/program_options.hpp>

class AnalysisParameters
{
public:
	const std::string csv_filename;
	const unsigned int delta_frame;
	const unsigned int same_colour_threshold;
	const unsigned int same_colour_level;
	const unsigned int delta_velocity;
	AnalysisParameters (const boost::program_options::variables_map &vm);
	static boost::program_options::options_description program_options ();
};

#endif // ANALYSISPARAMETERS_HPP
