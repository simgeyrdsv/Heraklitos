#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/string.hpp"
#include "agv_interfaces/srv/command.hpp"
#include "agv_interfaces/srv/learn_weight.hpp"
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>

#include <string>
#include <mutex>

class MainNode : public rclcpp::Node {
public:
    MainNode() : Node("main_node") , tf_buffer_(std::make_shared<rclcpp::Clock>(RCL_SYSTEM_TIME)){
        cb_group1_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        rclcpp::SubscriptionOptions options1;
        options1.callback_group = cb_group1_;

        start_button_subscriber_ = this->create_subscription<std_msgs::msg::Bool>(
            "start_button_state", 10, std::bind(&MainNode::start_button_callback, this, std::placeholders::_1),options1);

        close_button_subscriber_ = this->create_subscription<std_msgs::msg::Bool>(
            "close_button_state", 10, std::bind(&MainNode::close_button_callback, this, std::placeholders::_1),options1);

        qr_code_subscriber_ = this->create_subscription<std_msgs::msg::String>(
            "qr_code", 10, std::bind(&MainNode::qr_code_callback, this, std::placeholders::_1),options1);

        left_obstacle_subscriber_ = this->create_subscription<std_msgs::msg::Bool>(
            "left_obstacle", 10, std::bind(&MainNode::left_obstacle_callback, this, std::placeholders::_1),options1);

        right_obstacle_subscriber_ = this->create_subscription<std_msgs::msg::Bool>(
            "right_obstacle", 10, std::bind(&MainNode::right_obstacle_callback, this, std::placeholders::_1),options1);

        // Create a single Command service
        web_command_service_ = this->create_service<agv_interfaces::srv::Command>(
            "command", std::bind(&MainNode::command_callback, this, std::placeholders::_1, std::placeholders::_2));

        // Create service client for sending commands
        deneyap_command_client_ = this->create_client<agv_interfaces::srv::Command>("send_command");

        // Create service client for sending commands
        learn_weight_service_ = this->create_client<agv_interfaces::srv::LearnWeight>("learn_weight");
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(tf_buffer_);

        autonomous_driving_ = false;
        previous_qr_code_ = "";
        calibration_setting_ = false;
        left_obstacle_state_ = true;
        right_obstacle_state_ = true;
        left_obstacle_loop_=false;
        right_obstacle_loop_=false;
        rotation_detection_variable_ =false;
        scenario_status_=0;
        weight_lift_state = false;
        high_weight_state = false;
        weight_ok_state  = false;
        obstacle_callback_status = false;

        RCLCPP_INFO(this->get_logger(), "main_node node has been started");
    }

private:
    void start_button_callback(const std_msgs::msg::Bool::SharedPtr msg) {
        if (!msg->data && !autonomous_driving_) {
            RCLCPP_INFO(this->get_logger(), "Starting autonomous driving");
            // send_request_function_1("led_on");
            autonomous_driving_ = true;
            send_request_function_1("led_on");
            if(!calibration_setting_){
                send_request_function_1("follow_line");
            }
        }
    }

    void close_button_callback(const std_msgs::msg::Bool::SharedPtr msg) {
        if (msg->data) {
            calibration_setting_ = false;
            send_request_function_1("calibrate");
            std::this_thread::sleep_for(std::chrono::seconds(5));     
        }
    }

    void left_obstacle_callback(const std_msgs::msg::Bool::SharedPtr msg) {
        if (!msg->data && left_obstacle_loop_) {
            RCLCPP_WARN(this->get_logger(), "Weight obstacle detected");
            left_obstacle_state_ = msg->data;
            // RCLCPP_INFO(this->get_logger(), "Left obstacle state: %b", left_obstacle_state_);
            left_obstacle_loop_=false;
            if(weight_lift_state == true){
                send_request_function_1("stop_follow_line");
                yuk_alma();
                weight_lift_state = false;
            }
        }
        else if(msg->data){
            left_obstacle_loop_=true;
            left_obstacle_state_ = msg->data;
        }
    }

    void right_obstacle_callback(const std_msgs::msg::Bool::SharedPtr msg) {
        if (!msg->data && right_obstacle_loop_) {
            RCLCPP_WARN(this->get_logger(), "Right obstacle detected");
            right_obstacle_state_ = msg->data;
            right_obstacle_loop_ = false;
            obstacle_callback();
        }
        else if(msg->data){
            right_obstacle_loop_=true;
            right_obstacle_state_ = !msg->data;
        }
        if(obstacle_callback_status){
            obstacle_callback_status = false;
            send_request_function_1("turn_off_melodious_ringtone");
            if(!right_obstacle_state_ ){
                RCLCPP_INFO(this->get_logger(), "Obstacle is removed");
                send_request_function_1("follow_line");
            }
            else{
                // RCLCPP_INFO(this->get_logger(), "Obstacle not removed");
                // send_request_function_1("turn_left_90");
                // std::this_thread::sleep_for(std::chrono::seconds(3)); 
                // controlLoop(0.4);
                // std::this_thread::sleep_for(std::chrono::seconds(3)); 
                // send_request_function_1("turn_right_90");
                // std::this_thread::sleep_for(std::chrono::seconds(3)); 
                // controlLoop(0.9);
                // std::this_thread::sleep_for(std::chrono::seconds(3)); 
                // send_request_function_1("turn_right_90");
                // std::this_thread::sleep_for(std::chrono::seconds(3)); 
                // controlLoop(0.4);
                // std::this_thread::sleep_for(std::chrono::seconds(3)); 
                // send_request_function_1("turn_left_90");
                // std::this_thread::sleep_for(std::chrono::seconds(3));
                // send_request_function_1("stop"); 
            }
        }
    }

    void obstacle_callback(){
        if(autonomous_driving_){
            send_request_function_1("stop_follow_line");
        }
        else{
            send_request_function_1("stop");
        }
        RCLCPP_INFO(this->get_logger(), "Waiting for removed obstacle");
        send_request_function_1("turn_on_melodious_ringtone");
        std::this_thread::sleep_for(std::chrono::seconds(15));
        obstacle_callback_status = true;
    }

    void qr_code_callback(const std_msgs::msg::String::SharedPtr msg) {
        if(msg->data == previous_qr_code_){
            return;
        }
        else{
            RCLCPP_INFO(this->get_logger(), "QR Code detected: %s", msg->data.c_str());
            previous_qr_code_= msg->data;
        }
        RCLCPP_INFO(this->get_logger(), "3333333");
        if(msg->data == "Q1;1000.0;0.0"){
            q1++;
            if(scenario_status_ == 1){
                if(q1 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                if(q1 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        }
        else if(msg->data == "Q2;1750.0;0.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        }
        else if(msg->data == "Q3;3750.0;0.0"){
            q3 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q3 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                if(q3 == 1)
                    sola_dogru();
            }
        } 
        else if(msg->data == "Q4;4375.0;0.0"){
            q4 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
            }
            else if(scenario_status_ == 5){
            }
            else if(scenario_status_ == 6){
                if(q4 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 7){  
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q5;6375.0;0.0"){
            q5++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                if(q5 == 2)
                    sola_dogru();
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q6;7000.0;0.0"){
              RCLCPP_INFO(this->get_logger(), "00000000");
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q7;8000.0;0.0"){
            q7++;
            if(scenario_status_ == 1){
            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                if(q7 == 1)
                    send_request_function_1("stop_follow_line");
            }
            else if(scenario_status_ == 4){
                if(q7 == 1)
                    send_request_function_1("stop_follow_line"); 
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                if(q7 == 3)
                    send_request_function_1("stop_follow_line");
            }
            else if(scenario_status_ == 8){
                if(q7 == 3)
                    send_request_function_1("stop_follow_line");
            }
        } 
        else if(msg->data == "Q8;9000.0;0.0"){
            q8++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q8 == 2)
                    sola_dogru();
            }
            else if(scenario_status_ == 6){
                if(q8 == 2)
                    sola_dogru();
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q9;9625.0;0.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
            }
            else if(scenario_status_ == 6){
                if(q9 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 7){
                if(q9 == 1)
                    saga_dogru(); 
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q10;11625.0;0.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q11;12250.0;0.0"){
            q11 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q11 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                if(q11 == 1)
                    saga_dogru();
            }
        } 
        else if(msg->data == "Q12;14250.0;0.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q13;15000.0;0.0"){
            q13++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                if(q13 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                if(q13 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q14;16000.0;1000.0"){
            q14++;
            if(scenario_status_ == 1){
                if(q14 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                if(q14 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q15;16000.0;11500.0"){
            q15++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                if(q15 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                if(q15 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q16;15000.0;12500.0"){
            q16++;
            if(scenario_status_ == 1){
                if(q16 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                if(q16 == 1)
                    sola_dogru();  
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q17;14250.0;12500.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
    } 
        else if(msg->data == "Q18;12250.0;12500.0"){
            q18++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                if(q18 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 7){
                if(q18 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q19;11625.0;12500.0"){
            q19++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                if(q19 == 1)
                    saga_dogru();
            }
        }
        else if(msg->data == "Q20;9625.0;12500.0"){
            q20 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q20 == 2)
                    sola_dogru();
            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        }
        else if(msg->data == "Q21;9000.0;12500.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q22;8000.0;12500.0"){
            q22++;
            if(scenario_status_ == 1){
                if(q22 == 1)
                    send_request_function_1("stop_follow_line");
            }
            else if(scenario_status_ == 2){
                if(q22 == 1)
                    send_request_function_1("stop_follow_line");
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q22 == 3)
                    send_request_function_1("stop_follow_line");
            }
            else if(scenario_status_ == 6){
                if(q22 == 3)
                    send_request_function_1("stop_follow_line");
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q23;7000.0;12500.0"){
            q23++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                if(q23 == 2)
                    sola_dogru();
            }
            else if(scenario_status_ == 8){
                if(q23 == 2)
                    sola_dogru();
            }
        } 
        else if(msg->data == "Q24;6375.0;12500.0"){
            q24 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q24 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                if(q24 == 1)
                    saga_dogru();
            }
        } 
        else if(msg->data == "Q25;4375.0;12500.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q26;3750.0;12500.0"){
            q26 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                if(q26 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 7){
                if(q26 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q27;1750.0;12500.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q28;1000.0;12500.0"){
            q28++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                if(q28 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                if(q28 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q29;0.0;11500.0"){
            q29++;
            if(scenario_status_ == 1){
                if(q29 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                if(q29 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q30;0.0;1000.0"){
            q30++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                if(q30 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                if(q30 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q31;2750.0;1000.0"){
            q31 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                if(q31 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 7){
                if(q31 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q32;2750.0;5250.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q33;2750.0;6250.0"){
            q33++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q33 == 1)
                    weight_lift_state = true;
            }
            else if(scenario_status_ == 6){
                if(q33 == 1)
                    yuk_birakma();
            }
            else if(scenario_status_ == 7){
                if(q33 == 1)
                    weight_lift_state = true;
            }
            else if(scenario_status_ == 8){
                if(q33 == 1)
                    yuk_birakma(); 
            }
        } 
        else if(msg->data == "Q34;2750.0;7250.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q35;2750.0;11500.0"){
            q35 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q35 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                if(q35 == 1)
                    sola_dogru(); 
            }
    } 
        else if(msg->data == "Q36;5375.0;11500.0"){
            q36++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                if(q36 == 1)
                    sola_dogru();        
            }
            else if(scenario_status_ == 7){
                if(q36 == 1)
                    sola_dogru();  
            }
            else if(scenario_status_ == 8){
                
            }
    } 
        else if(msg->data == "Q37;5375.0;7250.0"){

            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q38;5375.0;6250.0"){
            q38 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q38 == 1)
                    yuk_birakma();
            }
            else if(scenario_status_ == 6){
                if(q38 == 1)
                    weight_lift_state = true;
            }
            else if(scenario_status_ == 7){
                if(q38 == 1)
                    yuk_birakma();
            }
            else if(scenario_status_ == 8){
                if(q38 == 1)
                    weight_lift_state = true;
            }
        }
        else if(msg->data == "Q39;5375.0;5250.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        }
        else if(msg->data == "Q40;5375.0;1000.0"){
            q40 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q40 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                if(q40 == 1)
                    sola_dogru(); 
            }
        } 
        else if(previous_qr_code_ == "Q41;8000.0;1000.0"){
            q41 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q41 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 6){
                if(q41 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q42;8000.0;11500.0"){
            q42++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                if(q42 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 8){
                if(q42 == 1)
                    saga_dogru();
            }
        } 
        else if(msg->data == "Q43;10625.0;11500.0"){
            q43 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                if(q43 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 7){
                if(q43 == 1)
                    sola_dogru();  
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q44;10625.0;7250.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q45;10625.0;6250.0"){
            q45 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q45 == 1)
                    yuk_birakma();
            }
            else if(scenario_status_ == 6){
                if(q45 == 1)
                    weight_lift_state = true;   
            }
            else if(scenario_status_ == 7){
                if(q45 == 1)
                    yuk_birakma();
            }
            else if(scenario_status_ == 8){
                if(q45 == 1)
                    weight_lift_state = true;   
            }
        } 
        else if(msg->data == "Q46;10625.0;5250.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q47;10625.0;1000.0"){
            q47++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q47 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                if(q47 == 1)
                    sola_dogru();
            }
        } 
        else if(msg->data == "Q48;13250.0;1000.0"){
            q48++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                if(q48 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 7){
                if(q48 == 1)
                    sola_dogru();
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q49;13250.0;5250.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q50;13250.0;6250.0"){
            q50 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q50 == 1)
                    weight_lift_state = true;
            }
            else if(scenario_status_ == 6){
                if(q50 == 1)
                    yuk_birakma();
            }
            else if(scenario_status_ == 7){
                if(q50 == 1)
                    weight_lift_state = true;
            }
            else if(scenario_status_ == 8){
                if(q50 == 1)
                    yuk_birakma();
            }
        } 
        else if(msg->data == "Q51;13250.0;7250.0"){
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){

            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                
            }
        } 
        else if(msg->data == "Q52;13250.0;11500.0"){    
            q52 ++;
            if(scenario_status_ == 1){

            }
            else if(scenario_status_ == 2){
                
            }
            else if(scenario_status_ == 3){
                
            }
            else if(scenario_status_ == 4){
                
            }
            else if(scenario_status_ == 5){
                if(q52 == 1)
                    saga_dogru();
            }
            else if(scenario_status_ == 6){
                
            }
            else if(scenario_status_ == 7){
                
            }
            else if(scenario_status_ == 8){
                if(q52 == 1)
                    saga_dogru();   
            }
        } 
        rotation_detection_variable_=false;
        RCLCPP_INFO(this->get_logger(), "222222222222");
    }

    void command_callback(const std::shared_ptr<agv_interfaces::srv::Command::Request> request,
                        std::shared_ptr<agv_interfaces::srv::Command::Response> response) {

        if(request->command == "start_button"){
            auto msg = std::make_shared<std_msgs::msg::Bool>();
            msg->data = false;
            start_button_callback(msg);
            response->success=true;
        }
        else if(request->command == "close_button"){
            auto msg = std::make_shared<std_msgs::msg::Bool>();
            msg->data = true;
            close_button_callback(msg);
            response->success=true;
        }
        else if(request->command == "there_is_a_return"){
            rotation_detection_variable_=true;
            response->success=true;
        }
        else if(request->command == "high_weight"){
            high_weight_state=true;
            response->success=true;
        }
        else if(request->command == "weight_ok"){
            weight_ok_state =true;
            response->success=true;
        }
        else if(request->command == "senaryo1"){
            scenario_status_ = 1;
            response->success=true;
        }
        else if(request->command == "senaryo2"){
            scenario_status_ = 2;
            response->success=true;
        }
        else if(request->command == "senaryo3"){
            scenario_status_ = 3;
            response->success=true;
        }
        else if(request->command == "senaryo4"){
            scenario_status_ = 4;
            response->success=true;
        }
        else if(request->command == "senaryo5"){
            scenario_status_ = 5;
            response->success=true;
        }
        else if(request->command == "senaryo6"){
            scenario_status_ = 6;
            response->success=true;
        }
        else if(request->command == "senaryo7"){
            scenario_status_ = 7;
            response->success=true;
        }
        else if(request->command == "senaryo8"){
            scenario_status_ = 8;
            response->success=true;
        }
        else{
            response->success = send_request(request->command);
        }
    }

    bool send_request(const std::string &command) {

        auto request = std::make_shared<agv_interfaces::srv::Command::Request>();
        request->command = command;

        RCLCPP_INFO(this->get_logger(), "Waiting for service...");
        if (!deneyap_command_client_->wait_for_service(std::chrono::seconds(30))) {
            RCLCPP_ERROR(this->get_logger(), "Service not available after waiting");
            return false;
        }

        using ServiceResponseFuture = rclcpp::Client<agv_interfaces::srv::Command>::SharedFuture;

        // Değişkenleri değerle yakalayacak şekilde lambda fonksiyonunu güncelleyin
        auto response_received_callback = [this, command](ServiceResponseFuture future) {
            try {
                auto response = future.get();
                // RCLCPP_INFO(this->get_logger(), "Received response for command %s: %s", command.c_str(), response->success ? "Success" : "Failure");
            } catch (const std::exception &e) {
                RCLCPP_ERROR(this->get_logger(), "Service call failed: %s", e.what());
            }
        };

        auto future = deneyap_command_client_->async_send_request(request, response_received_callback);
        // RCLCPP_INFO(this->get_logger(), "Command sent: %s", command.c_str());
        return true;
    }


    void send_request_function_1(const std::string &mesaj) {
        if (send_request(mesaj)) {
            //RCLCPP_INFO(this->get_logger(), "'%s' command executed successfully", mesaj.c_str());
        } else {
            RCLCPP_ERROR(this->get_logger(), "Failed to execute '%s' command", mesaj.c_str());
        }
    }

    bool send_request_function_2(const std::string &mesaj) {
        if (send_request(mesaj)) {
            RCLCPP_INFO(this->get_logger(), "'%s' command executed successfully", mesaj.c_str());
            return true;
        } 
        else {
            RCLCPP_ERROR(this->get_logger(), "Failed to execute '%s' command", mesaj.c_str());
            return false;
        }
    }

    void controlLoop(float distance)
    {
        try
        {
            auto transform = tf_buffer_.lookupTransform("base_link", "map", tf2::TimePointZero);

            float start_x = transform.transform.translation.x;
            float current_x = start_x;
            send_request_function_1("stop_follow_line");
            send_request_function_1("forward");
            while (fabs(current_x - start_x) < distance)
            {
                // RCLCPP_INFO(this->get_logger(), "Distance traveled: %f", fabs(current_x - start_x));
                transform = tf_buffer_.lookupTransform("base_link", "map", tf2::TimePointZero);
                current_x = transform.transform.translation.x;
            }
            send_request_function_1("stop");
            RCLCPP_INFO(this->get_logger(), "Move finished");
        }
        catch (const tf2::TransformException &ex)
        {
            RCLCPP_ERROR(this->get_logger(), "Transform error: %s", ex.what());
        }
    }
    void saga_dogru(){
        send_request_function_1("open_rotation_detection_variable");
        while(!rotation_detection_variable_){

        }
        send_request_function_1("turn_right_with_qtr");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        send_request_function_1("follow_line");
        RCLCPP_INFO(this->get_logger(), "Saga dogru donuldu ... ");
    }
    void sola_dogru(){
        send_request_function_1("open_rotation_detection_variable");
        while(!rotation_detection_variable_){

        }
        send_request_function_1("turn_left_with_qtr");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        send_request_function_1("follow_line");
        RCLCPP_INFO(this->get_logger(), "Sola dogru donuldu ... ");
    }


    void yuk_alma(){
        send_request_function_1("increase_speed");
        send_request_function_1("linear_actuator_forward");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        while(1){
            if(high_weight_state){
                break;
            }
            else if(weight_ok_state){
                break;
            }
        }
        while(send_weight_request() > 125 ){
            send_request_function_1("turn_on_melodious_ringtone");
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        send_request_function_1("turn_off_melodious_ringtone");
        //arayüze gönder
        send_request_function_1("follow_line");     
        RCLCPP_INFO(this->get_logger(), "Yuk alma islemi tamamlandi ... ");
    }
    void yuk_birakma(){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        send_request_function_1("stop_follow_line");
        ///Yükleme işlemleri eklenecek
        std::this_thread::sleep_for(std::chrono::seconds(1));
        send_request_function_1("linear_actuator_backward");
        send_request_function_1("follow_line");   
        send_request_function_1("decrease_speed");   
        RCLCPP_INFO(this->get_logger(), "Yuk birakma islemi tamamlandi... ");
    }
    // Servis çağrısını bir callback içinde kullanarak
    float send_weight_request() {
        auto request = std::make_shared<agv_interfaces::srv::LearnWeight::Request>();

        RCLCPP_INFO(this->get_logger(), "Waiting for service...");
        if (!learn_weight_service_->wait_for_service(std::chrono::seconds(30))) {
            RCLCPP_ERROR(this->get_logger(), "Service not available after waiting");
            return -1;  // Hata durumunda geri dön
        }

        auto future = learn_weight_service_->async_send_request(request);

        // Bu kodu bir geri arama içinde kullandığınızda, `executor` zaten bu işlemi yönetecektir
        future.wait();  // Veya geleceği işlemek için farklı bir yöntem kullanabilirsiniz.

        if (future.valid()) {
            auto response = future.get();
            if (response->success) {
                RCLCPP_INFO(this->get_logger(), "Weight learning succeeded. Weight: %f", response->weight);
                return response->weight;
            } else {
                RCLCPP_WARN(this->get_logger(), "Weight learning failed.");
            }
        } else {
            RCLCPP_ERROR(this->get_logger(), "Service call failed.");
        }
    }


    rclcpp::CallbackGroup::SharedPtr cb_group1_;

    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr start_button_subscriber_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr close_button_subscriber_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr left_obstacle_subscriber_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr right_obstacle_subscriber_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr qr_code_subscriber_;

    rclcpp::Service<agv_interfaces::srv::Command>::SharedPtr web_command_service_;
    rclcpp::Client<agv_interfaces::srv::Command>::SharedPtr deneyap_command_client_;
    rclcpp::Client<agv_interfaces::srv::LearnWeight>::SharedPtr learn_weight_service_;

    tf2_ros::Buffer tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

    std::mutex mtx_1_;

    std::string previous_qr_code_;
    bool autonomous_driving_;
    bool calibration_setting_;
    bool left_obstacle_state_;
    bool right_obstacle_state_;
    bool left_obstacle_loop_;
    bool right_obstacle_loop_;
    bool rotation_detection_variable_;
    bool high_weight_state;
    bool weight_ok_state;
    bool weight_lift_state;
    int scenario_status_;
    bool obstacle_callback_status;


    int q1 = 0; 
    int q2 = 0; 
    int q3 = 0; 
    int q4 = 0; 
    int q5 = 0; 
    int q6 = 0; 
    int q7 = 0; 
    int q8 = 0; 
    int q9 = 0; 
    int q10 = 0; 
    int q11 = 0; 
    int q12 = 0; 
    int q13 = 0; 
    int q14 = 0; 
    int q15 = 0; 
    int q16 = 0; 
    int q17 = 0; 
    int q18 = 0; 
    int q19 = 0; 
    int q20 = 0;
    int q21 = 0;
    int q22 = 0; 
    int q23 = 0;
    int q24 = 0;
    int q25 = 0;
    int q26 = 0;
    int q27 = 0;
    int q28 = 0;
    int q29 = 0;
    int q30 = 0;
    int q31 = 0;
    int q32 = 0;
    int q33 = 0;
    int q34 = 0;
    int q35 = 0;
    int q36 = 0;
    int q37 = 0;
    int q38 = 0;
    int q39 = 0;
    int q40 = 0;
    int q41 = 0;
    int q42 = 0;
    int q43 = 0;
    int q44 = 0;
    int q45 = 0;
    int q46 = 0;
    int q47 = 0;
    int q48 = 0;
    int q49 = 0;
    int q50 = 0;
    int q51 = 0;
    int q52 = 0;

};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto main_node = std::make_shared<MainNode>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(main_node);
    executor.spin();
    
    rclcpp::shutdown();
    return 0;
}
