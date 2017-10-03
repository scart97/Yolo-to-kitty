#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

// Required by yolo
#define OPENCV
#include "darknet/src/yolo_v2_class.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/core/version.hpp>


void draw_boxes(cv::Mat mat_img, std::vector<bbox_t> detection_vec, std::vector<std::string> obj_names, 
	unsigned int wait_msec = 0)
{
	int const colors[6][3] = { { 1,0,1 },{ 0,0,1 },{ 0,1,1 },{ 0,1,0 },{ 1,1,0 },{ 1,0,0 } };

	for (auto &detection : detection_vec) {
		
		// Create unique color for each different class 
		int const offset = detection.obj_id * 123457 % 6;
		int const color_scale = 150 + (detection.obj_id * 123457) % 100;
		cv::Scalar color(colors[offset][0], colors[offset][1], colors[offset][2]);
		color *= color_scale;

		//draw detection on image
		cv::rectangle(mat_img, cv::Rect(detection.x, detection.y, detection.w, detection.h), color, 5);

		if (obj_names.size() > detection.obj_id) {
			std::string obj_name = obj_names[detection.obj_id];
			if (detection.track_id > 0) 
				obj_name += " - " + std::to_string(detection.track_id);
			
			cv::Size const text_size = getTextSize(obj_name, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, 2, 0);
			int const max_width = (text_size.width > detection.w + 2) ? text_size.width : (detection.w + 2);
			
			cv::rectangle(mat_img, cv::Point2f(std::max((int)detection.x - 3, 0), std::max((int)detection.y - 30, 0)), 
				cv::Point2f(std::min((int)detection.x + max_width, mat_img.cols-1), std::min((int)detection.y, mat_img.rows-1)), 
				color, CV_FILLED, 8, 0);
			
			putText(mat_img, obj_name, cv::Point2f(detection.x, detection.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 0), 2);
		}
	}
	
	cv::imshow("Detection", mat_img);
	cv::waitKey(wait_msec);
}


std::vector<std::string> objects_names_from_file(const std::string &filename) {
	std::ifstream file(filename);
	std::vector<std::string> file_lines;
	if (!file.is_open()) 
		return file_lines;
	for(std::string line; getline(file, line);) 
		file_lines.push_back(line);
	std::cout << "object names loaded \n";
	return file_lines;
}

void
save_detections_to_kitti(const std::vector<bbox_t> &detections, std::vector<std::string> obj_names, 
							const std::string &image_name, 
							const std::vector<std::string> &obj_name_filter)
{
	//this substr separates the string between the / and the extension
	//for example Images/dog.jpg becomes dog
	std::string file_name = image_name.substr(image_name.find_last_of("/") + 1, image_name.find_last_of(".") - image_name.find_last_of("/") - 1);
	std::string complete_filename = "labels/" + file_name + ".txt";

	std::cout << complete_filename << std::endl;

	std::ofstream file;
	file.open(complete_filename);

	for(const bbox_t object : detections)
	{
		std::string name = obj_names.at(object.obj_id);

		if(std::find(obj_name_filter.begin(), obj_name_filter.end(), name) == obj_name_filter.end())
			name = "other";
	
		file << name << " -1 -1 -10 " << object.x << " " << object.y << " " << object.x + object.w << " " << object.y + object.h << " -1 -1 -1 -1000 -1000 -1000 -10\n";
	}


	file.close();

}


void
add_desired_classes(std::vector<std::string> &vec)
{

	vec.emplace_back("person");
	vec.emplace_back("bicycle");
	vec.emplace_back("car");
	vec.emplace_back("motorbike");
	vec.emplace_back("aeroplane");
	vec.emplace_back("bus");
	vec.emplace_back("train");
	vec.emplace_back("truck");
	vec.emplace_back("traffic light");
	vec.emplace_back("stop sign");
	vec.emplace_back("cat");
	vec.emplace_back("dog");

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

	std::vector<std::string> obj_name_filter;
	add_desired_classes(obj_name_filter);

	std::ifstream file(filename);
	if (!file.is_open()) 
		std::cout << "File not found! \n";
	else 
		for (std::string line; file >> line;) {
			std::cout << line << std::endl;
			
			cv::Mat mat_img = cv::imread(line);
			std::vector<bbox_t> result_vec = detector.detect(mat_img);
			
			std::cout << "Number of detections: " << result_vec.size() << std::endl;

/**************UNCOMMENT THE FOLLOWING LINE TO SHOW DETECTIONS******************/
            //draw_boxes(mat_img, result_vec, obj_names);

			save_detections_to_kitti(result_vec, obj_names, line, obj_name_filter);

		}

	return 0;
}