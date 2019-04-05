
#ifndef CONFIGURATION_HEADER
#define CONFIGURATION_HEADER
// ****************************************************************************
// Module: Configuration Header
//
// Description:
//
//
// Required Packages:
//
// Acknoledgements:
//
// ****************************************************************************
#include <Global_Configuration.h>

#define MQTT_SENSOR_NAME      "MQTT_GARAGE_DOOR_001"

//Define the pins
#define DOOR_RELAY_PIN        D1
#define DOOR_SENSOR_PIN       15
 
// MQTT Topics
#define MQTT_BASE_TOPIC        "sensor/garage/"
#define MQTT_ALL_TOPICS        MQTT_BASE_TOPIC "#"
#define MQTT_DOOR_SENSOR_TOPIC MQTT_BASE_TOPIC "state1" 
#define MQTT_DOOR_RELAY_TOPIC  MQTT_BASE_TOPIC "action1"


// Enum to define the states of the door
enum class DoorState
{
  Unknown,
  Open,
  Closed
};

#endif // CONFIGURATION_HEADER
