project(hw1)
cmake_minimum_required(VERSION 2.8)

# find required libs in opencv
find_package(OpenCV REQUIRED)

# create sub-projects
add_executable(gui gui.cpp)
target_link_libraries(gui ${OpenCV_LIBS})

