#include "fitness-function.hpp"

namespace po = boost::program_options;


FitnessFunction::FitnessFunction (const po::variables_map &vm):
   evolutionary_parameters (vm),
   analysis_parameters (vm)
{

}

void FitnessFunction::compute_fitness ()
{

}

