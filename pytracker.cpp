#include <boost/python.hpp>
#include <opencv2/opencv.hpp>
#include "tracker/tracker.hpp"
#include <boost/python/numpy.hpp>
namespace p = boost::python;
namespace np = boost::python::numpy;

void display_image(){
    auto img = cv::imread("img.jpg");
    cv::circle(img,{100,100},100,{255,255,255},-1);
    cv::imwrite("img2.jpg",img);
    cv::imshow("IMG",img);
}

void print_np(np::ndarray& xy_detections){
    int num_detections = xy_detections.shape(0);
    float* det_ptr = reinterpret_cast<float*>(xy_detections.get_data());
    for (int i = 0;i<num_detections;++i){
        for (int j =0;j<4;++j) {
            std::cout << det_ptr[i*4 + j ] << " ";
        }
        std::cout<<std::endl;

    }
}

BOOST_PYTHON_MODULE(pytracker)
{
    using namespace boost::python;

    np::initialize();

    def("display_image", display_image);
    def("print_np",print_np);
    //def("CreateTracker",CreatePedestrianTracker);


}


//void example_tracker(){
//    std::unique_ptr<PedestrianTracker> tracker = CreatePedestrianTracker();
//    TrackedObjects detections;
//    int video_fps=0;
//    int frameIdx = 0;
//    cv::Mat frame;
//    uint64_t cur_timestamp = static_cast<uint64_t>(1000.0 / video_fps * frameIdx);
//    tracker->Process(frame, detections, cur_timestamp);
//}