#include "evolutionary-parameters.hpp"
#include "util.hpp"

using namespace std;
namespace po = boost::program_options;

#define PO_CSV_FILENAME "csv-file"
#define PO_FRAME_FILE_TYPE "frame-file-type"
#define PO_MASK_FILE_TYPE "mask-file-type"
#define PO_BACKGROUND_FILENAME "background-filename"
#define PO_NUMBER_ROIs "number-ROIs"
#define PO_NUMBER_FRAMES "number-frames"
#define PO_MASK_NUMBER_STARTS_AT_0 "mask-number-starts-at-1"
#define PO_FRAME_FILENAME_PREFIX "frame-filename-prefix"
#define PO_SUBFOLDER_FRAMES "subfolder-frames"
#define PO_SUBFOLDER_BACKGROUND "subfolder-background"
#define PO_SUBFOLDER_MASK "subfolder-mask"

EvolutionaryParameters::EvolutionaryParameters (const po::variables_map &vm):
   csv_filename (vm [PO_CSV_FILENAME].as<string> ()),
   frame_file_type (vm [PO_FRAME_FILE_TYPE].as<string> ()),
   mask_file_type (vm [PO_MASK_FILE_TYPE].as<string> ()),
   background_filename (vm [PO_BACKGROUND_FILENAME].as<string> ()),
   number_ROIs (vm [PO_NUMBER_ROIs].as<unsigned int> ()),
   number_frames (vm [PO_NUMBER_FRAMES].as<unsigned int> ()),
   mask_number_starts_at_0 (vm.count (PO_MASK_NUMBER_STARTS_AT_0) > 0),
   frame_filename_prefix (vm [PO_FRAME_FILENAME_PREFIX].as<string> ()),
   subfolder_frames (verify_slash_at_end_path (vm [PO_SUBFOLDER_FRAMES].as<string> ())),
   subfolder_background (verify_slash_at_end_path (vm [PO_SUBFOLDER_BACKGROUND].as<string> ())),
   subfolder_mask (verify_slash_at_end_path (vm [PO_SUBFOLDER_MASK].as<string> ()))
{

}

po::options_description EvolutionaryParameters::program_options ()
{
	po::options_description config ("Options that describe how the experiment was performed");
	config.add_options ()
	      (
	         PO_CSV_FILENAME",a",
	         po::value<string> ()
	         ->default_value ("data-analyse.csv")
	         ->value_name ("FILENAME"),
	         "CSV file with data of which folders to analyse"
	         )
	      (
	         PO_NUMBER_ROIs",r",
	         po::value<unsigned int> ()
	         ->default_value (3)
	         ->value_name ("N"),
	         "how many regions of interest exist"
	         )
	      (
	         PO_NUMBER_FRAMES",n",
	         po::value<unsigned int> ()
	         ->required ()
	         ->value_name ("N"),
	         "how many frames the videos have"
	         )
	      ;
	po::options_description logistic ("Options for describing how the files with image data are organised");
	logistic.add_options ()
	      (
	         PO_SUBFOLDER_FRAMES,
	         po::value<string> ()
	         ->default_value ("")
	         ->value_name ("PATH"),
	         "folder where frames are stored (this is added to the folder of each video to analyse)"
	         )
	      (
	         PO_FRAME_FILENAME_PREFIX,
	         po::value<string> ()
	         ->default_value ("frames-")
	         ->value_name ("NAME"),
	         "prefix of the frames filename"
	         )
	      (
	         PO_FRAME_FILE_TYPE",f",
	         po::value<string> ()
	         ->required ()
	         ->default_value ("png")
	         ->value_name ("EXTENSION"),
	         "file type of the frames"
	         )
	      (
	         PO_SUBFOLDER_MASK,
	         po::value<string> ()
	         ->default_value ("")
	         ->value_name ("PATH"),
	         "folder where masks are stored (this is added to the folder of each video to analyse)"
	         )
	      (
	         PO_MASK_FILE_TYPE,
	         po::value<string> ()
	         ->default_value ("png")
	         ->value_name ("EXTENSION"),
	         "file type of the masks"
	         )
	      (
	         PO_MASK_NUMBER_STARTS_AT_0,
	         "mask numbes start at zero (by default they start at one)"
	         )
	      (
	         PO_SUBFOLDER_BACKGROUND,
	         po::value<string> ()
	         ->default_value ("")
	         ->value_name ("PATH"),
	         "folder where the background image is stored (this is added to the folder of each video to analyse)"
	         )
	      (
	         PO_BACKGROUND_FILENAME,
	         po::value<string> ()
	         ->default_value ("background.png")
	         ->value_name ("NAME"),
	         "file name of the background image"
	         )
	      ;
	po::options_description result;
	result.add (config);
	result.add (logistic);
	return result;
}
