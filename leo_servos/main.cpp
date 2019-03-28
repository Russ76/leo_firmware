#include "hFramework.h"
#include "hCloudClient.h"
#include "ros.h"
#include "std_msgs/Int16.h"

#include "limits.h"

ros::NodeHandle nh;
ros::Subscriber<std_msgs::Int16> *servo1_sub;
ros::Subscriber<std_msgs::Int16> *servo2_sub;
ros::Subscriber<std_msgs::Int16> *servo3_sub;

void servo1Callback(const std_msgs::Int16& msg)
{
	hServo.servo1.rotAbs(msg.data);
	Serial.printf("[servo1Callback] angle: %d\r\n", msg.data);
}

void servo2Callback(const std_msgs::Int16& msg)
{
	hServo.servo2.rotAbs(msg.data);
	Serial.printf("[servo2Callback] angle: %d\r\n", msg.data);
}

void servo3Callback(const std_msgs::Int16& msg)
{
	hServo.servo3.rotAbs(msg.data);
	Serial.printf("[servo3Callback] angle: %d\r\n", msg.data);
}

void setupServos()
{
	hServo.enablePower(); 
	hServo.setVoltage7V4();

	hServo.servo1.calibrate(0, SERVO_1_MIN, 90, SERVO_1_MAX); 
	hServo.servo2.calibrate(0, SERVO_2_MIN, 90, SERVO_2_MAX); 
	hServo.servo3.calibrate(0, SERVO_3_MIN, 90, SERVO_3_MAX); 
}

void initROS()
{
	servo1_sub = new ros::Subscriber<std_msgs::Int16>("/servo1/command", &servo1Callback);
	servo2_sub = new ros::Subscriber<std_msgs::Int16>("/servo2/command", &servo2Callback);
	servo3_sub = new ros::Subscriber<std_msgs::Int16>("/servo3/command", &servo3Callback);
	nh.subscribe(*servo1_sub);
	nh.subscribe(*servo2_sub);
	nh.subscribe(*servo3_sub);
}

void hMain()
{
	uint32_t t = sys.getRefTime();
	platform.begin(&RPi);
	nh.getHardware()->initWithDevice(&platform.LocalSerial);
	nh.initNode();

	setupServos();
	initROS();

	while (true)
	{
		nh.spinOnce();
		sys.delaySync(t, 10);
	}
}