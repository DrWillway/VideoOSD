cmake_minimum_required(VERSION 3.10)
project(VideoOSD)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)

add_executable(main main.cpp VideoOSD/VideoOSD.cpp)
target_include_directories(main PRIVATE ${OpenCV_INCLUDE_DIRS} nlohmann_json_INCLUDE_DIRS)
target_link_libraries(main ${OpenCV_LIBS})

install(TARGETS main
        RUNTIME DESTINATION bin)