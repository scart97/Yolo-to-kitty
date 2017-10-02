#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <atomic>
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

#define OPENCV

#include "darknet/src/yolo_v2_class.hpp"	// imported functions from DLL

#include <opencv2/opencv.hpp>
#include "opencv2/core/version.hpp"


std::vector<std::string> objects_names_from_file(std::string const filename) {
	std::ifstream file(filename);
	std::vector<std::string> file_lines;
	if (!file.is_open()) 
		return file_lines;
	for(std::string line; getline(file, line);) 
		file_lines.push_back(line);
	std::cout << "object names loaded \n";
	return file_lines;
}


int main(int argc, char *argv[])
{
	std::string filename;
	if (argc > 1) 
		filename = argv[1];

    /**** YOLO PARAMETERS ****/
    std::string cfg_filename = "darknet/cfg/yolo.cfg";
    std::string weight_filename = "darknet/data/yolo.weights";
    std::string obj_names_filenames = "darknet/data/coco.names";
	auto obj_names = objects_names_from_file(obj_names_filenames);

	/**** Initialize network ****/
	Detector detector(cfg_filename, weight_filename);

	std::ifstream file(filename);
	if (!file.is_open()) 
		std::cout << "File not found! \n";
	else 
		for (std::string line; file >> line;) {
			std::cout << line << std::endl;
			
			cv::Mat mat_img = cv::imread(line);
			std::vector<bbox_t> result_vec = detector.detect(mat_img);

			/* TODO: Save result to kitti format*/

		}

	return 0;
}