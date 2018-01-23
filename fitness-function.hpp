#ifndef FITNESSFUNCTION_HPP
#define FITNESSFUNCTION_HPP

#include <boost/program_options.hpp>

#include "evolutionary-parameters.hpp"
#include "analysis-parameters.hpp"

class FitnessFunction
{
	const EvolutionaryParameters evolutionary_parameters;
	const AnalysisParameters analysis_parameters;
public:
	FitnessFunction (const boost::program_options::variables_map &vm);
	void compute_fitness ();
};

#endif // FITNESSFUNCTION_HPP
