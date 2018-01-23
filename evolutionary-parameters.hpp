#ifndef EVOLUTIONARYPARAMETERS_HPP
#define EVOLUTIONARYPARAMETERS_HPP

#include <boost/program_options.hpp>

/**
 * @brief The EvolutionaryParameters class contains parameters that describe how the data of several runs of the assisi-evovibe are stored.
 */
class EvolutionaryParameters
{
public:
	const std::string csv_filename;
	const std::string frame_file_type;
	const std::string mask_file_type;
	const std::string background_filename;
	const unsigned int number_ROIs;
	const unsigned int number_frames;
	const bool mask_number_starts_at_0;
	const std::string frame_filename_prefix;
	const std::string subfolder_frames;
	const std::string subfolder_background;
	const std::string subfolder_mask;
public:
	EvolutionaryParameters (const boost::program_options::variables_map &vm);
	static boost::program_options::options_description program_options ();
};

#endif // EVOLUTIONARYPARAMETERS_HPP
