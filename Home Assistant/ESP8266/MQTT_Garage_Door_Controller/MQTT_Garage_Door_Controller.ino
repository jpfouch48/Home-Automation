// ****************************************************************************
// Module: MQTT Garage Door Controller
//
// Description:
// 
// Required Packages:
//  PubSubClient    (2.7.0) Nick O'Leary
//  ESP8266Wifi     (1.0.0) Ivan Grokhotkov
//
// Acknoledgements:
//  https://github.com/Snipercaine/HA_Garage_Doors
//
// ****************************************************************************

#include <ESP8266SSDP.h>

// ** ESP8266Wifi
#include <ESP8266WiFi.h>

// ** PubSubClient
#include <PubSubClient.h>

// Local configuration header
#include "Configuration.h"
 
WiFiClient gWifiClient;
PubSubClient gMqttClient(gWifiClient);
DoorState gDoorState = DoorState::Unknown;


//Setup Variables
String switch1;
String strTopic;
String strPayload;

// **************************************************************************** 
// Routine: 
//
// Description:
//
// Arguments:
//
// **************************************************************************** 
void setup() 
{
  //Set Relay(output) and Door(input) pins
  pinMode(DOOR_RELAY_PIN, OUTPUT);
  digitalWrite(DOOR_RELAY_PIN, LOW); // was HIGH
  pinMode(DOOR_SENSOR_PIN, INPUT);
 
  Serial.begin(115200);

  wifi_setup();

  gMqttClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT); 
  gMqttClient.setCallback(mqtt_callback);
}

// **************************************************************************** 
// Routine: 
//
// Description:
//
// Arguments:
//
// **************************************************************************** 
void loop() 
{
  //If MQTT client can't connect to broker, then reconnect
  if (!gMqttClient.connected()) 
  {
    wifi_reconnect();
  }
  gMqttClient.loop();

  checkDoorState();
}
 
// **************************************************************************** 
// Routine: 
//
// Description:
//
// Arguments:
//
// **************************************************************************** 
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
  //if the 'garage/button' topic has a payload "OPEN", then 'click' the relay
  payload[length] = '\0';
  strTopic = String((char*)topic);

  //Serial.println(strTopic);

  if (strTopic == MQTT_DOOR_RELAY_TOPIC)
  {
    switch1 = String((char*)payload);
    Serial.println(switch1);
    if (switch1 == "OPEN")
    {
      //'click' the relay
      Serial.println("ON");
      digitalWrite(DOOR_RELAY_PIN, HIGH); // was LOW
      delay(600);
      digitalWrite(DOOR_RELAY_PIN, LOW);  // was HIGH
    }
  }
}

// **************************************************************************** 
// Routine: 
//
// Description:
//
// Arguments:
//
// **************************************************************************** 
void wifi_setup() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_SSID_PW);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi connected - ");
  Serial.println(WiFi.localIP());
}

// **************************************************************************** 
// Routine: 
//
// Description:
//
// Arguments:
//
// **************************************************************************** 
void checkDoorState() 
{
  DoorState lPrevDoorState = gDoorState;

  if (digitalRead(DOOR_SENSOR_PIN) == 0)      gDoorState = DoorState::Closed;
  else if (digitalRead(DOOR_SENSOR_PIN) == 1) gDoorState = DoorState::Open; 

  if (gDoorState != lPrevDoorState) 
  {
    if(gDoorState == DoorState::Closed)
    {
      gMqttClient.publish(MQTT_DOOR_SENSOR_TOPIC, "CLOSED", true);
      Serial.println("CLOSED");
    }
    else
    {
      gMqttClient.publish(MQTT_DOOR_SENSOR_TOPIC, "OPEN", true);
      Serial.println("OPENED");
    }
  }
}

// **************************************************************************** 
// Routine: 
//
// Description:
//
// Arguments:
//
// **************************************************************************** 
void wifi_reconnect() 
{
  // Loop until we're reconnected
  while (!gWifiClient.connected()) 
  {
    Serial.println("Attempting MQTT connection...");

    // Attempt to connect
    if (gMqttClient.connect(MQTT_SENSOR_NAME, MQTT_USER, MQTT_USER_PW)) 
    {
      Serial.println("connected");
      gMqttClient.subscribe(MQTT_ALL_TOPICS);      
    } 
    else 
    {
      Serial.print("failed, retry again in 5 seconds rc=");
      Serial.println(gMqttClient.state());
      delay(5000);
    }
  }
}
