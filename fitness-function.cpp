#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

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
	this->create_file_fitness_recomputed ();
	while (videos_analyse.next ()) {
		CSVReader::CSVRow row = videos_analyse.row ();
		const string &base_folder = row.column (0);
		const string &pixel_count_difference_filename = row.column (1);
		cout << base_folder << '\n';
		if (!this->ok_file_pixel_count_difference (pixel_count_difference_filename)) {
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
			this->create_file_pixel_count_difference (pixel_count_difference_filename);
			for (unsigned int frame_index = 1; frame_index <= this->evolutionary_parameters.number_frames; frame_index++) {
				snprintf (frame_filename, length_frame_filename, frame_template, frame_index);
				Image current_frame_normal = read_image (base_folder + frame_filename);
				Image current_frame_HE;
				cv::equalizeHist (current_frame_normal, current_frame_HE);
				this->compute_pixel_count_difference (background_HE, current_frame_HE);
			}
			this->close_file_pixel_count_difference (pixel_count_difference_filename);
		}
		int new_fitness = 0;
		CSVReader pixel_count_difference_csv (pixel_count_difference_filename);
		pixel_count_difference_csv.next (); // skip header row
		for (unsigned int frame_index = 1; frame_index < this->analysis_parameters.first_frame; frame_index++) {
			pixel_count_difference_csv.next ();
		}
		for (unsigned int frame_index = this->analysis_parameters.first_frame; frame_index <= this->analysis_parameters.last_frame; frame_index++) {
			pixel_count_difference_csv.next ();
			CSVReader::CSVRow row = pixel_count_difference_csv.row ();
			unsigned int previous_active = std::stoi (row.column (1));
			unsigned int previous_passive = std::stoi (row.column (3));
			if (previous_active < this->analysis_parameters.PCD_previous_threshold)
				new_fitness++;
			if (previous_passive < this->analysis_parameters.PCD_previous_threshold)
				new_fitness--;
		}
		fprintf (this->fitness_recomputed_file, "%s,%s,%s,%s,%s,%s,%s,%s,%d\n",
		         row.column (2).c_str (),
		         row.column (3).c_str (),
		         row.column (4).c_str (),
		         row.column (5).c_str (),
		         row.column (6).c_str (),
		         row.column (7).c_str (),
		         row.column (8).c_str (),
		         row.column (9).c_str (),
		         new_fitness);
	}
	fclose (this->fitness_recomputed_file);
}

void FitnessFunction::create_file_fitness_recomputed ()
{
	const char fitness_recomputed_template[] = "fitness-recomputed_SCT=%d_PCDPT=%d.csv";
	const unsigned int length_fitness_recomputed_filename = strlen (fitness_recomputed_template) + 4 + 4 + 1;
	char fitness_recomputed_filename [length_fitness_recomputed_filename];
	snprintf (
	         fitness_recomputed_filename,
	         length_fitness_recomputed_filename,
	         fitness_recomputed_template,
	         this->analysis_parameters.same_colour_threshold,
	         this->analysis_parameters.PCD_previous_threshold);
	this->fitness_recomputed_file = fopen (fitness_recomputed_filename, "w");
	fprintf (this->fitness_recomputed_file, "\"run\",\"generation\",\"bee.set\",\"iteration\",\"frequency\",\"amplitude\",\"pause\",\"old.fitness\",\"new.fitness\"\n");

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

