
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

#define MQTT_SENSOR_NAME        "MQTT_LED_STRIP_001"

#define MQTT_BASE_TOPIC         "led/" MQTT_SENSOR_NAME

#define MQTT_SUB_SET_COLOR      MQTT_BASE_TOPIC "/setcolor"
#define MQTT_SUB_SET_POWER      MQTT_BASE_TOPIC "/setpower"
#define MQTT_SUB_SET_EFFECT     MQTT_BASE_TOPIC "/seteffect"
#define MQTT_SUB_SET_BRIGHTNESS MQTT_BASE_TOPIC "/setbrightness"
#define MQTT_SUB_ANIMATION_SPEED MQTT_BASE_TOPIC "/setanimationspeed"

#define MQTT_PUB_SET_COLOR      MQTT_BASE_TOPIC "/setcolorpub"
#define MQTT_PUB_SET_POWER      MQTT_BASE_TOPIC "/setpowerpub"
//#define MQTT_PUB_SET_EFFECT     MQTT_BASE_TOPIC "/seteffectpub"
//#define MQTT_PUB_SET_BRIGHTNESS MQTT_BASE_TOPIC "/setbrightnesspub"
//#define MQTT_PUB_COLOR_STATUS   MQTT_BASE_TOPIC "/colorstatus"

#define DATA_PIN      D5 
#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB
#define NUM_LEDS      300

#endif // CONFIGURATION_HEADER
