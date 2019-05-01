#include "bluefox2/single_node.h"
#include "sensor_msgs/TimeReference.h"

void time_ref_callback(sensor_msgs::TimeReference _tref) {
  shutter_stamp_last = _tref.time_ref;
  ROS_INFO("recv time %6.1f ms, sht t %6.1fms %6.1fms",
    fmod(ros::Time::now().toSec()*1000,100000),
    fmod(_tref.header.stamp.toSec()*1000, 100000),
    fmod(_tref.time_ref.toSec()*1000, 100000)
  );  
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "bluefox2_single");
  ros::NodeHandle pnh("~");
  ros::Subscriber tref_sub = pnh.subscribe("/dji_sdk_1/dji_sdk/trigger_time", 1, time_ref_callback, ros::TransportHints().tcpNoDelay());
  try {
    bluefox2::SingleNode single_node(pnh);
    single_node.Run();
    ros::spin();
    single_node.End();
  }
  catch (const std::exception &e) {
    ROS_ERROR("%s: %s", pnh.getNamespace().c_str(), e.what());
  }
}
