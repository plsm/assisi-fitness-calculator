#ifndef FITNESSFUNCTION_HPP
#define FITNESSFUNCTION_HPP

#include <boost/program_options.hpp>
#include <vector>
#include <queue>

#include "evolutionary-parameters.hpp"
#include "analysis-parameters.hpp"

#include "image.hpp"

class FitnessFunction
{
	const EvolutionaryParameters evolutionary_parameters;
	const AnalysisParameters analysis_parameters;
	std::vector<Image> ROIs;
	std::queue<Image> cache;
	FILE *file;
	FILE *fitness_recomputed_file;
	void compute_pixel_count_difference (const Image &background, const Image &current_frame);
	/**
	 * @brief ok_file_pixel_count_difference Checks if the file with pixel count difference data exists and is write-protected.
	 * Failure means the computation of the data did not finished.
	 * @param path
	 * @return false if the file does not exist or is not write protected.
	 */
	bool ok_file_pixel_count_difference (const std::string &path);
	/**
	 * @brief create_file_pixel_count_difference Creates the file that will contain the pixel count difference data.
	 * The data is stored in CSV format.
	 * There is a header row.
	 * @param path
	 */
	void create_file_pixel_count_difference (const std::string &path);
	void close_file_pixel_count_difference (const std::string &path);

	void create_file_fitness_recomputed ();
public:
	FitnessFunction (const boost::program_options::variables_map &vm);
	void compute_fitness ();
};

#endif // FITNESSFUNCTION_HPP
