#cmake_minimum_required(VERSION 3.16)
project(tracker)
#
set(CMAKE_CXX_STANDARD 14)

IF(UNIX AND NOT WIN32)
    FIND_PROGRAM(CMAKE_UNAME uname /bin /usr/bin /usr/local/bin )
    IF(CMAKE_UNAME)
        EXEC_PROGRAM(uname ARGS -m OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
        SET(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE INTERNAL
                "processor type (i386 and x86_64)")
        IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
            ADD_DEFINITIONS(-fPIC)
        ENDIF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    ENDIF(CMAKE_UNAME)
ENDIF(UNIX AND NOT WIN32)

cmake_minimum_required(VERSION 3.5)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
# Find python and Boost - both are required dependencies
find_package(PythonLibs 3 REQUIRED)
find_package(Boost COMPONENTS python numpy REQUIRED)
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Without this, any build libraries automatically have names "lib{x}.so"
set(CMAKE_SHARED_MODULE_PREFIX "")

# Add a shared module - modules are intended to be imported at runtime.
# - This is where you add the source files

add_library(pyctracker_wrapper STATIC tracker_wrapper.cpp tracker_wrapper.h)
add_library(pyctracker MODULE pyctracker.cpp)

# Set up the libraries and header search paths for this target
include_directories(tracker)
link_directories(tracker)
add_subdirectory(tracker)

target_link_libraries(pyctracker_wrapper ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${OpenCV_LIBS} TRACKER)
target_include_directories(pyctracker_wrapper PRIVATE ${PYTHON_INCLUDE_DIRS})


target_link_libraries(pyctracker ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${OpenCV_LIBS} pyctracker_wrapper TRACKER)
target_include_directories(pyctracker PRIVATE ${PYTHON_INCLUDE_DIRS})



