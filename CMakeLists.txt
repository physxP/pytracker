#cmake_minimum_required(VERSION 3.16)
project(tracker)
#
set(CMAKE_CXX_STANDARD 14)



cmake_minimum_required(VERSION 3.5)

# Find python and Boost - both are required dependencies
find_package(PythonLibs 3 REQUIRED)
find_package(Boost COMPONENTS python numpy REQUIRED)
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Without this, any build libraries automatically have names "lib{x}.so"
set(CMAKE_SHARED_MODULE_PREFIX "")

# Add a shared module - modules are intended to be imported at runtime.
# - This is where you add the source files
add_library(pytracker MODULE pytracker.cpp)
add_library(pytracker_wrapper tracker_wrapper.cpp tracker_wrapper.h)



# Set up the libraries and header search paths for this target
add_subdirectory(tracker)
target_link_libraries(pytracker ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${OpenCV_LIBS})
target_include_directories(pytracker PRIVATE ${PYTHON_INCLUDE_DIRS})


