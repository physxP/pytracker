cmake_minimum_required(VERSION 3.16)
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

#set(Boost_INCLUDE_DIR /home/ahmed/Downloads/boost_1_73_0)
#set(Boost_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/boost_1_73_0/)
set(Boost_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/boost/include)

message(${Boost_INCLUDE_DIR})
##set(Boost_LIBRARY_DIR /home/ahmed/Downloads/boost_1_73_0/stage/lib)
#set(Boost_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/boost_1_73_0/stage/lib)
set(Boost_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/boost/lib)

#set(BOOST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/boost_1_73_0)
set(BOOST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/boost)

#set()

#message(${CMAKE_CURRENT_SOURCE_DIR})
find_package(Boost COMPONENTS python38 numpy38 REQUIRED)

#set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/opencv_install/build)
#set(OpenCV_INSTALL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/opencv_install)
find_package(OpenCV REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/opencv_install_gtk)


include_directories( ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} )
message(${OpenCV_INCLUDE_DIRS})


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



