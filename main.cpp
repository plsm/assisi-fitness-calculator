#include <iostream>
#include <boost/program_options.hpp>

#include "analysis-parameters.hpp"
#include "evolutionary-parameters.hpp"
#include "fitness-function.hpp"

using namespace std;
namespace po = boost::program_options;

static po::variables_map process_options (int argc, char *argv[]);

int main (int argc, char *argv[])
{
	po::variables_map vm = process_options (argc, argv);
	FitnessFunction fit (vm);
	fit.compute_fitness ();
	return 0;
}

static po::options_description program_options ()
{
	po::options_description result ("Process a set of videos of bee behaviour to compute fitness value.\n\nAvailable options");
	result.add_options ()
	      (
	         "help,h",
	         "show this help message"
	         )
	      ;
	result.add (EvolutionaryParameters::program_options ());
	result.add (AnalysisParameters::program_options ());
	return result;
}

static po::variables_map process_options (int argc, char *argv[])
{
	po::options_description options = program_options ();
	po::variables_map vm;
	po::store (po::parse_command_line (argc, argv, options), vm);
	po::notify (vm);
	if (vm.count ("help")) {
		cout << options << "\n";
		exit (EXIT_SUCCESS);
	}
	return vm;
}
