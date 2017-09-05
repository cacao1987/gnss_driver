/******************************************************************************
 * Original work Copyright 2017 The Apollo Authors. All Rights
 * Reserved.  
 * 
 * Modified work Copyright 2017 Board of Regents of the
 * Nevada System of Higher Education, on behalf of the University of
 * Nevada, Reno. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>
#include <ros/ros.h>
#include <std_msgs/String.h>

#include "data_parser.h"
#include "gnss/parser.h"

namespace gnss_driver {

  class ParserNodelet : public nodelet::Nodelet {
  public:
    ParserNodelet() {}
    ~ParserNodelet() {}

  private:
    virtual void onInit();
    
    std::unique_ptr<DataParser> data_parser_;
  };

  void ParserNodelet::onInit() {
    ros::NodeHandle& nh = getPrivateNodeHandle();
    
    std::string gnss_conf;
    std::string raw_data_topic;
    std::string gpgga_topic;
    std::string corr_imu_topic;
    std::string odometry_topic;
    std::string gnss_status_topic;
    std::string ins_status_topic;

    nh.param("gnss_conf", gnss_conf, std::string("./conf/gnss_conf.txt"));
    nh.param("raw_data_topic", raw_data_topic, std::string("/gnss_driver/raw_data"));
    nh.param("gpgga_topic", gpgga_topic, std::string("/gnss_driver/gpgga"));
    nh.param("corr_imu_topic", corr_imu_topic, std::string("/gnss_driver/corrected_imu"));
    nh.param("odometry_topic", odometry_topic, std::string("/gnss_driver/odometry"));
    nh.param("gnss_status_topic", gnss_status_topic, std::string("/gnss_driver/gnss_status"));
    nh.param("ins_status_topic", ins_status_topic, std::string("/gnss_driver/ins_status"));

    data_parser_.reset(new DataParser(nh, raw_data_topic, gpgga_topic, corr_imu_topic,
				      odometry_topic, gnss_status_topic, ins_status_topic));

    if(!data_parser_->init(gnss_conf)) {
      ROS_ERROR("Init parser nodelet failed.");
      ROS_ERROR_STREAM("Init parser nodelet failed.");
      return;
    }

    ROS_INFO("Init parser nodelet success.");    
  }  
}

PLUGINLIB_DECLARE_CLASS(gnss_driver, ParserNodelet,
			gnss_driver::ParserNodelet, nodelet::Nodelet);
