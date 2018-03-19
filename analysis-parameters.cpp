#include "analysis-parameters.hpp"
#include "image.hpp"

namespace po = boost::program_options;
using namespace std;

#define PO_CSV_FILENAME "csv-file"
#define PO_DELTA_FRAME "delta-frame"
#define PO_SAME_COLOUR_THRESHOLD "same-colour-threshold"
#define PO_FIRST_FRAME "first-frame"
#define PO_LAST_FRAME "last-frame"
#define PO_PCD_PREVIOUS_THRESHOLD "PCD-previous-threshold"
#define PO_DELTA_VELOCITY "delta-velocity"

AnalysisParameters::AnalysisParameters (const po::variables_map &vm):
   csv_filename (vm [PO_CSV_FILENAME].as<string> ()),
   delta_frame (vm [PO_DELTA_FRAME].as<unsigned int> ()),
   same_colour_threshold (vm [PO_SAME_COLOUR_THRESHOLD].as<unsigned int> ()),
   same_colour_level (round ((NUMBER_COLOUR_LEVELS * same_colour_threshold) / 100.0)),
   first_frame (vm [PO_FIRST_FRAME].as<unsigned int> ()),
   last_frame (vm [PO_LAST_FRAME].as<unsigned int> ()),
   PCD_previous_threshold (vm [PO_PCD_PREVIOUS_THRESHOLD].as<unsigned int> ()),
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
	         PO_FIRST_FRAME,
	         po::value<unsigned int> ()
	         ->required ()
	         ->value_name ("N"),
	         "index of the first frame used to compute fitness"
	         )
	      (
	         PO_LAST_FRAME,
	         po::value<unsigned int> ()
	         ->required ()
	         ->value_name ("N"),
	         "index of the last frame used to compute fitness"
	         )
	      (
	         PO_PCD_PREVIOUS_THRESHOLD,
	         po::value<unsigned int> ()
	         ->required ()
	         ->value_name ("T"),
	         "threshold used in the fitness function"
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

