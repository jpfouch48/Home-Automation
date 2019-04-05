// ****************************************************************************
// Module: MQTT IR Receive Controller
//
// Description:
// 
// Required Packages:
//  IRremoteESP8266 (2.5.6) Sebastien Warin, Mark Szabo, Ken Shirriff, 
//                          David Conran
//  ESP8266Wifi     (1.0.0) Ivan Grokhotkov
//  PubSubClient    (2.7.0) Nick O'Leary
//
// Acknoledgements:
//
// ****************************************************************************
#include <Arduino.h>

// ** IRremoteESP8266
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

// ** ESP8266Wifi
#include <ESP8266WiFi.h>

// ** PubSubClient
#include <PubSubClient.h>

// Local configuration header
#include "Configuration.h"

// ****************************************************************************
// Globals
// ****************************************************************************
IRrecv         gIrRecv(IR_RECV_PIN, IR_BUFFER_SIZE, IR_TIMEOUT, true);
decode_results gIrResult;
WiFiClient     gWifiClient;
PubSubClient   gMqttClient(gWifiClient);

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
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  
  Serial.begin(IR_BAUD_RATE, SERIAL_8N1, SERIAL_TX_ONLY);

  // Wait for serial connection
  while (!Serial)
  {
    delay(50);
  }

  gIrRecv.enableIRIn();

  wifi_setup();
  gMqttClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  gMqttClient.setCallback(mqtt_callback);

  Serial.println("Ready");
  Serial.print("IP address: ");
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
void wifi_setup() 
{
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
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
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
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
  // TODO: Handle any data in
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
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (gMqttClient.connect(MQTT_SENSOR_NAME, MQTT_USER, MQTT_USER_PW)) 
    {
      Serial.println("connected");
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(gMqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
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
void loop() 
{
  if (!gWifiClient.connected()) 
  {
    wifi_reconnect();
  }
  gMqttClient.loop();
    
  // Check if the IR code has been received.
  if (gIrRecv.decode(&gIrResult)) 
  {
    if(gIrResult.decode_type == MAGIQUEST)
    { 
      // Display a crude timestamp.
      uint32_t now = millis();
      Serial.printf("Timestamp : %06u.%03u\n", now / 1000, now % 1000);
      
      if (results.overflow)
      {
        Serial.printf(
            "WARNING: IR code is too big for buffer (>= %d). "
            "This result shouldn't be trusted until this is resolved. "
            "Edit & increase IR_BUFFER_SIZE.\n",
            IR_BUFFER_SIZE);
      }

      Serial.print(resultToHumanReadableBasic(&gIrResult));
    }
  }
}
