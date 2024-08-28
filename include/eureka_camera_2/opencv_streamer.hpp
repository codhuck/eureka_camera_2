#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

namespace opencvstreamer
{
class OpenCVStreamer : public rclcpp::Node {
public:
  OpenCVStreamer();
  ~OpenCVStreamer();

private:
  void streamCallback();

  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr pub1;
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr pub2;
  rclcpp::TimerBase::SharedPtr timer;
  cv::VideoCapture vid1;
  cv::VideoCapture vid2;
};
}