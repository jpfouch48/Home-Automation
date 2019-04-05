
#ifndef GLOBAL_CONFIGURATION_HEADER
#define GLOBAL_CONFIGURATION_HEADER
// ****************************************************************************
// Module: Global Configuration Header
//
// Description:
//  This file was created to break out any common configuration items that we
//  don't want to reside in our git repositories for security reasons. The
//  idea is that all the base code will be clean of things like passwords and
//  any other defines that we don't want published. A generic file will be
//  placed in the repo so that users can update the file locally without
//   exposing any secure data.
// 
//  ** This file must reside in a locatin that arduino can see it.
//     e.g. ~/Library/Global_Configuration/Global_Configuration.Header
//
// Required Packages:
//
// Acknoledgements:
//
// ****************************************************************************

// ****************************************************************************
// WIFI Defines
//  WIFI_SSID     - SSID of wireless network you want to connect to
//  WIFI_SSID_PW  - Password of wireless network  
// ****************************************************************************
#define WIFI_SSID         "TODO_WIFI_SSID"
#define WIFI_SSID_PW      "TODO_WIFI_SSID_PW"

// ****************************************************************************
// MQTT Defines
// ****************************************************************************
#define MQTT_SERVER_IP    "192.168.1.20"
#define MQTT_SERVER_PORT  1883
#define MQTT_USER         "TODO_MQTT_USER"
#define MQTT_USER_PW      "TODO_MQTT_USER_PW"


// ****************************************************************************
// OTA Defines
// ****************************************************************************
#define OTA_PW            ""
#define OTA_PORT          8266

#endif // GLOBAL_CONFIGURATION_HEADER
