//
// Created by mautisim on 12/7/20.
//

#include "tracker_wrapper.h"
#include "tracker/tracker.hpp"
#include "tracker/utils.hpp"
#include "tracker/descriptor.hpp"
#include "tracker/distance.hpp"
#include "tracker/logging.hpp"

tracker_wrapper::tracker_wrapper(float video_fps) {
    this->video_fps = video_fps;
    this->frameIdx = 0;
    this->tracker = CreatePedestrianTracker();
}

tracker_wrapper::tracker_wrapper(const tracker_wrapper& old){
    video_fps =  old.video_fps ;
    frameIdx = old.frameIdx;
    detections = old.detections;
//    tracker( std::move(old.tracker));
}

np::ndarray tracker_wrapper::update(np::ndarray& xy_detections,np::ndarray& confidence,np::ndarray& video_frame){
    int num_detections = xy_detections.shape(0);
    float* det_ptr = reinterpret_cast<float*>(xy_detections.get_data());
    float* conf_ptr = reinterpret_cast<float*>(confidence.get_data());

    TrackedObject tmp;
    this->detections.clear();

    for (int i = 0;i<num_detections;++i){
            tmp.rect.x = det_ptr[i*4 + 0];
            tmp.rect.y = det_ptr[i*4 + 1];
            tmp.rect.width =  det_ptr[i*4 + 2] - tmp.rect.x ;
            tmp.rect.height =  det_ptr[i*4 + 3] - tmp.rect.y ;
            tmp.confidence = conf_ptr[i];

            if (tmp.rect.area()>0)
                this->detections.emplace_back(tmp);
    }
        int rows = video_frame.shape(0);
        int columns = video_frame.shape(1);

        cv::Mat frame(rows,columns,CV_8UC3,video_frame.get_data());
        uint64_t cur_timestamp = static_cast<uint64_t>(1000.0 / video_fps * frameIdx);
        tracker->Process(frame, detections, cur_timestamp);
        int* object_ids = new int [num_detections];
        for (int i = 0;i<num_detections;++i){
            object_ids[i]= detections[i].object_id;
        }
        p::object own;
    ++frameIdx;

    return np::from_data(object_ids, np::dtype::get_builtin<int>(),p::make_tuple(num_detections),p::make_tuple(sizeof(int)),own);

}


std::unique_ptr<PedestrianTracker> CreatePedestrianTracker() {
    TrackerParams params;



    std::unique_ptr<PedestrianTracker> tracker(new PedestrianTracker(params));



    std::shared_ptr<IImageDescriptor> descriptor_fast =
            std::make_shared<ResizedImageDescriptor>(
                    cv::Size(16, 32), cv::InterpolationFlags::INTER_LINEAR);
    std::shared_ptr<IDescriptorDistance> distance_fast =
            std::make_shared<MatchTemplateDistance>();



    tracker->set_descriptor_fast(descriptor_fast);
    tracker->set_distance_fast(distance_fast);



    return tracker;
}
