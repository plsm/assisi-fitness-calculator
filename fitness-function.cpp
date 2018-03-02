#include <vector>
#include <string>

#include <iostream>

#include "fitness-function.hpp"
#include "csv.hpp"
#include "histogram.hpp"
#include "image.hpp"

namespace po = boost::program_options;

using namespace std;

FitnessFunction::FitnessFunction (const po::variables_map &vm):
   evolutionary_parameters (vm),
   analysis_parameters (vm),
   ROIs (evolutionary_parameters.number_ROIs)
{

}

void FitnessFunction::compute_fitness ()
{
	CSVReader videos_analyse (this->analysis_parameters.csv_filename);
	const char frame_template[] = "frame-%04d.png";
	const unsigned int length_frame_filename = strlen (frame_template) + 4 + 1;
	char frame_filename [length_frame_filename + 1];
	const char ROI_template[] = "../arena-1/Mask-%d.jpg";
	const unsigned int length_ROI_filename = strlen (ROI_template) + 4 + 1;
	char ROI_filename [length_ROI_filename + 1];
	string previous_base_folder;
	while (videos_analyse.next ()) {
		CSVReader::CSVRow row = videos_analyse.row ();
		const string &base_folder = row.column (0);
		cout << base_folder << '\n';
		Image background_normal = read_image (base_folder + this->evolutionary_parameters.background_path);
		Image background_HE;
		cv::equalizeHist (background_normal, background_HE);
		if (base_folder != previous_base_folder) {
			for (unsigned int ROI_index = 0; ROI_index < this->evolutionary_parameters.number_ROIs; ROI_index++) {
				snprintf (ROI_filename, length_ROI_filename, ROI_template, ROI_index);
				ROIs [ROI_index] = read_image (base_folder + ROI_filename);
			}
			previous_base_folder = base_folder;
		}
		while (!this->cache.empty ()) {
			this->cache.pop ();
		}
		this->create_file_pixel_count_difference (row.column (1));
		for (unsigned int frame_index = 1; frame_index <= this->evolutionary_parameters.number_frames; frame_index++) {
			snprintf (frame_filename, length_frame_filename, frame_template, frame_index);
			Image current_frame_normal = read_image (base_folder + frame_filename);
			Image current_frame_HE;
			cv::equalizeHist (current_frame_normal, current_frame_HE);
			this->compute_pixel_count_difference (background_HE, current_frame_HE);
		}
		fclose (this->file);
	}
}

void FitnessFunction::compute_pixel_count_difference (const cv::Mat &background, const cv::Mat &current_frame)
{
	static Histogram histogram;
	static cv::Mat number_bees, bee_speed;
	cv::absdiff (background, current_frame, number_bees);
	bool enough_frames = this->cache.size () > this->analysis_parameters.delta_frame;
	if (enough_frames) {
		cv::Mat previous_frame = this->cache.front ();
		this->cache.pop ();
		cv::absdiff (previous_frame, current_frame, bee_speed);
	}
	for (unsigned int index_mask = 0; index_mask < this->evolutionary_parameters.number_ROIs; index_mask++) {
		if (index_mask > 0) {
			fprintf (this->file, ",");
		}
		compute_histogram (number_bees, this->ROIs [index_mask], histogram);
		fprintf (file, "%d", histogram.number_different_pixels (this->analysis_parameters.same_colour_level));
		if (enough_frames) {
			compute_histogram (bee_speed, this->ROIs [index_mask], histogram);
			fprintf (this->file, ",%d", histogram.number_different_pixels (this->analysis_parameters.same_colour_level));
		}
		else {
			fprintf (this->file, ",-1");
		}
	}
	fprintf (this->file, "\n");
	this->cache.push (current_frame);
}

bool FitnessFunction::ok_file_pixel_count_difference (const string &path)
{
	return
	      (access (path.c_str (), F_OK) == 0) &&
	      (access (path.c_str (), W_OK) == -1);
}

void FitnessFunction::create_file_pixel_count_difference (const string &path)
{
	this->file = fopen (path.c_str (), "w");
	for (unsigned int index_mask = 1; index_mask <= this->evolutionary_parameters.number_ROIs; index_mask++) {
		if (index_mask > 1) {
			fprintf (this->file, ",");
		}
		fprintf (this->file, "\"NB%d\",\"BMT%d\"", index_mask, index_mask);
	}
	fprintf (this->file, "\n");
}

void FitnessFunction::close_file_pixel_count_difference (const string &path)
{
	fclose (file);
	chmod (path.c_str (), S_IRUSR);
}
