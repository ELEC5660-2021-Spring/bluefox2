#include "bluefox2/single_node.h"
#include "bluefox2/bluefox2_ros.h"
ros::Time shutter_stamp_last;
namespace bluefox2 {

SingleNode::SingleNode(const ros::NodeHandle& pnh)
    : CameraNodeBase(pnh),
      bluefox2_ros_(boost::make_shared<Bluefox2Ros>(pnh)) {}

void SingleNode::Acquire() {
  std::string mode;
  pnh().param<std::string>("mode", mode, "");
  bool use_sync = mode == "slave";

  while (is_acquire() && ros::ok()) {
    bluefox2_ros_->RequestSingle();
    const auto expose_us = bluefox2_ros_->camera().GetExposeUs();
    ros::Duration expose_duration = ros::Duration(expose_us * 1e-6 / 2);
    ros::Time time = ros::Time::now() - ros::Duration(0.03) + expose_duration;      
    
    if (use_sync) {
      ROS_INFO("Use sync time");
      time = shutter_stamp_last + expose_duration;
    }

    // if (use_sync) {
      // time = shutter_stamp_last + expose_duration;
    // }

    ROS_INFO("Now-sht %4.1fms", (ros::Time::now() - time).toSec()*1000);
    bluefox2_ros_->PublishCamera(time);
    Sleep();
  }
}

void SingleNode::AcquireOnce() {
  if (is_acquire() && ros::ok()) {
    bluefox2_ros_->RequestSingle();
    const auto expose_us = bluefox2_ros_->camera().GetExposeUs();
    const auto expose_duration = ros::Duration(expose_us * 1e-6 / 2);
    const auto time = ros::Time::now() + expose_duration;
    bluefox2_ros_->PublishCamera(time);
  }
}

void SingleNode::Setup(Bluefox2DynConfig& config) {
  bluefox2_ros_->set_fps(config.fps);
  bluefox2_ros_->camera().Configure(config);
}

}  // namepace bluefox2
