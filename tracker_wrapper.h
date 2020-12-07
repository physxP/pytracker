//
// Created by mautisim on 12/7/20.
//

#ifndef TRACKER_TRACKER_WRAPPER_H
#define TRACKER_TRACKER_WRAPPER_H
#include "tracker/tracker.hpp"
#include "tracker/utils.hpp"
#include "tracker/descriptor.hpp"
#include "tracker/distance.hpp"
#include "tracker/logging.hpp"

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
    tracker_wrapper(float video_fps);
    np::ndarray update(np::ndarray& xy_detections,np::ndarray& confidence,np::ndarray& video_frame);
    tracker_wrapper(const tracker_wrapper& old);
};

#endif //TRACKER_TRACKER_WRAPPER_H
