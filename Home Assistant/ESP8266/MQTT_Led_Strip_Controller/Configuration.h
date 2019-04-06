
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

#define MQTT_SENSOR_NAME          "MQTT_LED_STRIP_001"

#define MQTT_BASE_TOPIC           "led/" MQTT_SENSOR_NAME

#define MQTT_SUB_SET_COLORR       MQTT_BASE_TOPIC "/setcolorr"
#define MQTT_SUB_SET_COLORG       MQTT_BASE_TOPIC "/setcolorg"
#define MQTT_SUB_SET_COLORB       MQTT_BASE_TOPIC "/setcolorb"
#define MQTT_SUB_SET_POWER        MQTT_BASE_TOPIC "/setpower"
#define MQTT_SUB_SET_EFFECT       MQTT_BASE_TOPIC "/seteffect"
#define MQTT_SUB_SET_BRIGHTNESS   MQTT_BASE_TOPIC "/setbrightness"
#define MQTT_SUB_ANIMATION_SPEED  MQTT_BASE_TOPIC "/setanimationspeed"

#define MQTT_PUB_SET_COLORR       MQTT_BASE_TOPIC "/setcolorrpub"
#define MQTT_PUB_SET_COLORG       MQTT_BASE_TOPIC "/setcolorgpub"
#define MQTT_PUB_SET_COLORB       MQTT_BASE_TOPIC "/setcolorbpub"
#define MQTT_PUB_SET_POWER        MQTT_BASE_TOPIC "/setpowerpub"
#define MQTT_PUB_SET_EFFECT       MQTT_BASE_TOPIC "/seteffectpub"
#define MQTT_PUB_SET_BRIGHTNESS   MQTT_BASE_TOPIC "/setbrightnesspub"
#define MQTT_PUB_ANIMATION_SPEED  MQTT_BASE_TOPIC "/setanimationspeedpub"

#define DATA_PIN      D5 
#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB
#define NUM_LEDS      300

#endif // CONFIGURATION_HEADER
