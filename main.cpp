#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include "VideoOSD/VideoOSD.h"

int main(int argc, char** argv){
    std::string camera_device = "/dev/video0";
    bool reverse = false;

    // --- Parse command line arguments ---
    for(int i=1; i<argc; i++){
        std::string arg = argv[i];
        if(arg == "--camera" && i+1 < argc) {
            camera_device = argv[++i];
        }
        if(arg == "-cr") {
            reverse = true;
        }
    }

    // --- Load JSON config ---
    nlohmann::json j;
    try{
        std::ifstream f("config.json");
        if(!f.is_open()) throw std::runtime_error("File not found");
        f >> j;
    } catch(...) {
        std::cerr << "Cannot read config.json, using defaults\n";
        j = {
            {"resolution", {640, 480}},
            {"text", "OSD Text"},
            {"color", {0, 255, 0}},
            {"speed", 1.0}
        };
    }

    // --- Apply config values ---
    OSDConfig cfg;
    cfg.resolution = cv::Size(j["resolution"][0], j["resolution"][1]); // width, height
    cfg.text = j["text"];
    cfg.color = cv::Scalar(j["color"][2], j["color"][1], j["color"][0]); // BGR
    cfg.speed = j["speed"];

    // --- Process JSON args ---
    if(j.contains("args") && j["args"].is_array()){
        if(!j["args"].empty()){
            // first element is camera device
            camera_device = j["args"][0].get<std::string>();
        }
        for(size_t i=1; i<j["args"].size(); i++){
            std::string arg = j["args"][i].get<std::string>();
            if(arg == "-cr") {
                reverse = true;
            }
        }
    }

    // --- Run app ---
    try {
        VideoOSD app(cfg, reverse);
        app.run(camera_device); 
    } catch(std::exception& e){
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
