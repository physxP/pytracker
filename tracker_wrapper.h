//
// Created by mautisim on 12/7/20.
//

#ifndef TRACKER_TRACKER_WRAPPER_H
#define TRACKER_TRACKER_WRAPPER_H
#include "tracker/tracker.hpp"
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
namespace p = boost::python;
namespace np = boost::python::numpy;

std::unique_ptr<PedestrianTracker> CreatePedestrianTracker();

class tracker_wrapper {
private:
    std::unique_ptr<PedestrianTracker> tracker;
    TrackedObjects detections;
    float video_fps;
    int frameIdx;

public:
    void update(np::ndarray xy_detections,np::ndarray confidence);



};


#endif //TRACKER_TRACKER_WRAPPER_H
