// ****************************************************************************
// Module: MQTT IR Receive Controller
//
// Description:
// 
// Required Packages:
//
// Acknoledgements:
//
// ****************************************************************************
#include <Arduino.h>

#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// ==================== IR SETTINGS ====================
#define ir_RecvPin           2
#define ir_BaudRate          115200
#define ir_CaptureBufferSize 1024
#define ir_Timeout           15
#define ir_MinUnknownSize    12
// ==================== END IR SETTINGS ================

// ==================== WIFI SETTINGS ====================
#define wifi_ssid            "[SSID]"
#define wifi_password        "[SSID_PW]"
// ==================== END WIFI SETTINGS ================

// ==================== MQTT SETTINGS ====================
#define mqtt_server          "[MQTT_SERVER]"
#define mqtt_port            1883
#define mqtt_user            "[MQTT_USER]"
#define mqtt_password        "[MQTT_PW]"

#define mqtt_sensor_name     "[MQTT_SENSOR_NAME"
// ==================== END MQTT SETTINGS ================


// IR
IRrecv         ir_recv(ir_RecvPin, ir_CaptureBufferSize, ir_Timeout, true);
decode_results ir_results;

// WIFI
WiFiClient     wifi_client;

// MQTT
PubSubClient   mqtt_client(wifi_client);

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
  
  Serial.begin(ir_BaudRate, SERIAL_8N1, SERIAL_TX_ONLY);

  // Wait for serial connection
  while (!Serial)
    delay(50);

  ir_recv.enableIRIn();

  wifi_setup();
  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_client.setCallback(mqtt_callback);

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
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
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

}

// **************************************************************************** 
// Routine: 
//
// Description:
//
// Arguments:
//
// **************************************************************************** 
void wifi_reconnect() {
  // Loop until we're reconnected
  while (!wifi_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
  if (mqtt_client.connect(mqtt_sensor_name, mqtt_user, mqtt_password)) {
      Serial.println("connected");

//      FastLED.clear (); //Turns off startup LEDs after connection is made
//      FastLED.show();

//      client.subscribe(setcolorsub);
//      client.subscribe(setbrightness);
      //client.subscribe(setcolortemp);
//      client.subscribe(setpowersub);
//      client.subscribe(seteffectsub);
//      client.subscribe(setanimationspeed);
//      client.publish(setpowerpub, "OFF");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
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
  if (!wifi_client.connected()) {
    wifi_reconnect();
  }
  mqtt_client.loop();
    
  // Check if the IR code has been received.
  if (ir_recv.decode(&ir_results)) 
  {
    if(ir_results.decode_type == MAGIQUEST)
    { 
      // Display a crude timestamp.
      uint32_t now = millis();
      Serial.printf("Timestamp : %06u.%03u\n", now / 1000, now % 1000);
      
//      if (results.overflow)
//      {
//        Serial.printf(
//            "WARNING: IR code is too big for buffer (>= %d). "
//            "This result shouldn't be trusted until this is resolved. "
//            "Edit & increase kCaptureBufferSize.\n",
//            ir_CaptureBufferSize);
//      }

      Serial.print(resultToHumanReadableBasic(&ir_results));
    }
  }
}
