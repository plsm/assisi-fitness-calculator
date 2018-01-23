#include "analysis-parameters.hpp"
#include "image.hpp"

namespace po = boost::program_options;

#define PO_CSV_FILENAME "csv-file"
#define PO_DELTA_FRAME "delta-frame"
#define PO_SAME_COLOUR_THRESHOLD "same-colour-threshold"
#define PO_DELTA_VELOCITY "delta-velocity"

AnalysisParameters::AnalysisParameters (const po::variables_map &vm):
   csv_filename (vm [PO_CSV_FILENAME].as<string> ()),
   delta_frame (vm [PO_DELTA_FRAME].as<unsigned int> ()),
   same_colour_threshold (vm [PO_SAME_COLOUR_THRESHOLD].as<unsigned int> ()),
   same_colour_level (round ((NUMBER_COLOUR_LEVELS * same_colour_threshold) / 100.0)),
   delta_velocity (vm [PO_DELTA_VELOCITY].as<unsigned int> ())
{

}

po::options_description AnalysisParameters::program_options ()
{
	po::options_description file ("Options that describe which files to analyse");
	file.add_options ()
	      (
	         PO_CSV_FILENAME",a",
	         po::value<string> ()
	         ->default_value ("data-analyse.csv")
	         ->value_name ("FILENAME"),
	         "CSV file with data on which videos to analyse"
	         )
	      ;
	po::options_description analysis ("Options for the parameters that affect the analysis");
	analysis.add_options ()
	      (
	         PO_DELTA_FRAME",d",
	         po::value<unsigned int> ()
	         ->required ()
	         ->default_value (2)
	         ->value_name ("D"),
	         "how many frames apart are used when computing bee movement"
	         )
	      (
	         PO_SAME_COLOUR_THRESHOLD",c",
	         po::value<unsigned int> ()
	         ->required ()
	         ->value_name ("PERC"),
	         "threshold value used when deciding if two colour intensities are equal, percentage value"
	         )
	      (
	         PO_DELTA_VELOCITY",v",
	         po::value<unsigned int> ()
	         ->required ()
	         ->default_value (2)
	         ->value_name ("V"),
	         "how many frames apart are used when computing bee acceleration"
	         )
	      ;
	po::options_description result;
	result.add (file);
	result.add (analysis);
	return result;
}

