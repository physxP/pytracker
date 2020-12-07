//
// Created by mautisim on 12/7/20.
//

#include "tracker_wrapper.h"
#include "tracker/tracker.hpp"

void tracker_wrapper::update(np::ndarray xy_detections,np::ndarray confidence){
    int num_detections = xy_detections.shape(0);
    float* det_ptr = reinterpret_cast<float*>(xy_detections.get_data());
    float* conf_ptr = reinterpret_cast<float*>(confidence.get_data());

    for (int i = 0;i<num_detections;++i){
            TrackedObject tmp;
            tmp.rect.x = det_ptr[i*4 + 0];
            tmp.rect.y = det_ptr[i*4 + 1];
            tmp.rect.width =  det_ptr[i*4 + 2] - tmp.rect.x ;
            tmp.rect.height =  det_ptr[i*4 + 3] - tmp.rect.y ;
            tmp.confidence = conf_ptr[i];
            this->detections.emplace_back(tmp);


    }
        cv::Mat frame;
        uint64_t cur_timestamp = static_cast<uint64_t>(1000.0 / video_fps * frameIdx);
        tracker->Process(frame, detections, cur_timestamp);


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
