#include "eureka_camera_2/opencv_streamer.hpp"

namespace opencvstreamer
{
OpenCVStreamer::OpenCVStreamer() : Node("opencv_streamer") {
  pub1 = this->create_publisher<sensor_msgs::msg::Image>("/micro1/image_raw", 10);
  pub2 = this->create_publisher<sensor_msgs::msg::Image>("/micro2/image_raw", 10);

  timer= this->create_wall_timer(
      std::chrono::milliseconds(100), std::bind(&OpenCVStreamer::streamCallback, this));

  RCLCPP_INFO(this->get_logger(), "Camera %s started", "cam1");
}

OpenCVStreamer::~OpenCVStreamer() {
  vid1.release();
  vid2.release();
}

void OpenCVStreamer::streamCallback() {
  vid1.open("/dev/micro1", cv::CAP_V4L2);
  vid1.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('Y', 'U', 'Y', 'V'));
  vid1.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  vid1.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  vid1.set(cv::CAP_PROP_FPS, 10);

  cv::Mat frame;
  bool ret = vid1.read(frame);
  if (ret) {
    auto msg1 = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame).toImageMsg();
    pub1->publish(*msg1);
    RCLCPP_INFO(this->get_logger(), "Image 1 published!");
  }
  vid1.release();

  vid2.open("/dev/micro2", cv::CAP_V4L2);
  vid2.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('Y', 'U', 'Y', 'V'));
  vid2.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  vid2.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  vid2.set(cv::CAP_PROP_FPS, 1);

  ret = vid2.read(frame);
  if (ret) {
    auto msg2 = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame).toImageMsg();
    pub2->publish(*msg2);
    RCLCPP_INFO(this->get_logger(), "Image 2 published!");
  }
  vid2.release();
}
}

  int main(int argc, char** argv)
  {
    rclcpp::init(argc, argv);


    std::shared_ptr<opencvstreamer::OpenCVStreamer> node = std::make_shared<opencvstreamer::OpenCVStreamer>();

    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();

    return 0;
  }