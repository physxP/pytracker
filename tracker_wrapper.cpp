//
// Created by mautisim on 12/7/20.
//

#include "tracker_wrapper.h"
#include "tracker/tracker.hpp"
#include "tracker/utils.hpp"
#include "tracker/descriptor.hpp"
#include "tracker/distance.hpp"
#include "tracker/logging.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui_c.h>
#include "opencv2/imgproc.hpp"

tracker_wrapper::tracker_wrapper(float video_fps) {
    this->video_fps = video_fps;
    this->frameIdx = 0;
    this->tracker = CreatePedestrianTracker();
}

tracker_wrapper::tracker_wrapper(const tracker_wrapper& old){
    video_fps =  old.video_fps ;
    frameIdx = old.frameIdx;
    detections = old.detections;
//    tracker = CreatePedestrianTracker();
//    tracker( std::move(old.tracker));
}

np::ndarray tracker_wrapper::update(np::ndarray& xy_detections,np::ndarray& confidence,np::ndarray& video_frame){

    int num_detections = xy_detections.shape(0);
//    std::cout<<num_detections<<std::endl;
    float* det_ptr = reinterpret_cast<float*>(xy_detections.get_data());
    float* conf_ptr = reinterpret_cast<float*>(confidence.get_data());

    TrackedObject tmp;
    this->detections.clear();

//    std::cout<<det_ptr[0]<<','<<det_ptr[1]<<','<<det_ptr[2]<<','<<det_ptr[3]<<std::endl;

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

//    cvShowImage("window", frame);
//    cv::imshow("window", frame);

    uint64_t cur_timestamp = static_cast<uint64_t>(1000.0 / video_fps * frameIdx);
//        std::cout<<cur_timestamp<<std::endl;
//        std::cout<<this->detections[0].rect.x<<','<<detections[0].rect.y<<','<<detections[0].rect.width<<','<<detections[0].rect.height<<std::endl;

    tracker.Process(frame, this->detections, cur_timestamp);
//        std::cout<< this->detections[0].object_id<<std::endl;
    TrackedObjects dets = tracker.TrackedDetections();

    int* object_ids = new int32_t [(dets.size()*5)];

    for (int i = 0;i<dets.size();++i){
        object_ids[i*5]= dets[i].object_id;
        object_ids[i*5 + 1] = dets[i].rect.x;
        object_ids[i*5 + 2] = dets[i].rect.y;
        object_ids[i*5 + 3] = dets[i].rect.width + dets[i].rect.x;
        object_ids[i*5 + 4] = dets[i].rect.height + dets[i].rect.y;
    }
//    std::cout<<dets.size()<<' ';
//    std::cout<<std::endl;
    p::object own;
    ++frameIdx;


        //std::cout<<detections[0].object_id<<std::endl;

    return np::from_data(object_ids, np::dtype::get_builtin<int32_t>(),p::make_tuple(dets.size()*5),p::make_tuple(sizeof(int32_t)),own);

}


PedestrianTracker CreatePedestrianTracker() {
    TrackerParams params;



    PedestrianTracker tracker(params);
//    (PedestrianTracker(params));



    std::shared_ptr<IImageDescriptor> descriptor_fast =
            std::make_shared<ResizedImageDescriptor>(
                    cv::Size(16, 32), cv::InterpolationFlags::INTER_LINEAR);

//    IImageDescriptor *descriptor_fast;
//    ResizedImageDescriptor b(cv::Size(16, 32), cv::InterpolationFlags::INTER_LINEAR);
//    descriptor_fast = &b;


    std::shared_ptr<IDescriptorDistance> distance_fast =
            std::make_shared<MatchTemplateDistance>();

//    IDescriptorDistance *distance_fast;
//    MatchTemplateDistance a;
//    distance_fast = &a;

//    tracker.set_descriptor_fast(descriptor_fast);
//    tracker.set_distance_fast(distance_fast);




    tracker.set_descriptor_fast(descriptor_fast);
    tracker.set_distance_fast(distance_fast);



    return tracker;
}
