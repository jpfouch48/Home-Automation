// ****************************************************************************
// Module: MQTT Led Strip Controller
//
// Description:
// 
// Required Packages:
//  PubSubClient    (2.7.0) Nick O'Leary
//  FastLED         (3.2.6) Daniel Garcia
//  ESP8266Wifi     (1.0.0) Ivan Grokhotkov
//
// Acknoledgements:
//  https://github.com/Snipercaine/Holiday-LED-files
//
// ****************************************************************************

// ** ESP8266Wifi
#include <ESP8266WiFi.h>

// ** PubSubClient
#include <PubSubClient.h>

// ** FastLED
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

// Local configuration headers
#include "Configuration.h"
#include "Palette.h"

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

String  gEffect = "Solid";
int     gBrightness = 150;
int     gAnimationSpeed = 150;
bool    gEnabled = false;
uint8_t gBeatsPerMinute = 62;
uint8_t gStartIndex = 0;
int     gColorR = 150;
int     gColorG = 0;
int     gColorB = 0;

CRGB leds[NUM_LEDS];

// Custom Paletts
CRGBPalette16 gPaletteCandyCane;
CRGBPalette16 gPaletteHail;
CRGBPalette16 gPaletteThanksgiving;
CRGBPalette16 gPaletteHalloween;
CRGBPalette16 gPaletteHolyJoly;
CRGBPalette16 gPaletteIndependance;
CRGBPalette16 gPaletteFire;

/****************FOR NOISE - I'm using this one for Easter***************/
static uint16_t dist = 0;     // A random number for our noise generator.
uint16_t scale = 30;          // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges = 48;      // Value for blending between palettes.
CRGBPalette16 targetPalette(OceanColors_p);
CRGBPalette16 currentPalette(CRGB::Black);

/*****************For TWINKLE********/
#define DENSITY     80
int twinklecounter = 0;

/*********FOR RIPPLE***********/
uint8_t colour;                                               // Ripple colour is randomized.
int center = 0;                                               // Center of the current ripple.
int step = -1;                                                // -1 is the initializing step.
uint8_t myfade = 255;                                         // Starting brightness.
#define maxsteps 16                                           // Case statement wouldn't allow a variable.
uint8_t bgcol = 0;                                            // Background colour rotates.
int thisdelay = 20;                                           // Standard delay value.

/**************FOR RAINBOW***********/
uint8_t thishue = 0;                                          // Starting hue value.
uint8_t deltahue = 10;

/**************FOR DOTS**************/
uint8_t   count =   0;                                        // Count up to 255 and then reverts to 0
uint8_t fadeval = 224;                                        // Trail behind the LED's. Lower => faster fade.
uint8_t bpm = 30;

/**************FOR LIGHTNING**************/
uint8_t frequency = 50;                                       // controls the interval between strikes
uint8_t flashes = 8;                                          //the upper limit of flashes per strike
unsigned int dimmer = 1;
uint8_t ledstart;                                             // Starting location of a flash
uint8_t ledlen;
int lightningcounter = 0;

/********FOR FUNKBOX EFFECTS**********/
int idex = 0;                //-LED INDEX (0 to NUM_LEDS-1
int TOP_INDEX = int(NUM_LEDS / 2);
int thissat = 255;           //-FX LOOPS DELAY VAR

//////////////////add thishue__ for Police All custom effects here/////////////////////////////////////////////////////////
/////////////////use hsv Hue number for one color, for second color change "thishue__ + __" in the setEffect section//////

uint8_t thishuepolice = 0;
uint8_t thishuehail = 64;
uint8_t thishueLovey = 0;     
int antipodal_index(int i) {
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) {
    iN = ( i + TOP_INDEX ) % NUM_LEDS;
  }
  return iN;
}

/********FIRE**********/
#define COOLING  55
#define SPARKING 120
bool gReverseDirection = false;

/********BPM**********/
uint8_t gHue = 0;
char message_buff[100];

WiFiClient gWifiClient; //this needs to be unique for each controller
PubSubClient gMqttClient(gWifiClient); //this needs to be unique for each controller

////////////////////////////////////////////////////////////
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
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(12, 10000); //experimental for power management. Feel free to try in your own setup.
  FastLED.setBrightness(gBrightness);

  gPaletteCandyCane       = getStripedPalette        ( CRGB::Red, CRGB::Red, CRGB::White, CRGB::White);
  gPaletteThanksgiving    = getThanksgivingPalette   ( CRGB::OrangeRed, CRGB::Olive, CRGB::Maroon, CRGB::Maroon);
  gPaletteHail            = getHailPalette           ( CRGB::Blue, CRGB::Blue, CRGB::Yellow, CRGB::Yellow);
  gPaletteHalloween       = getHalloweenPalette      ( CRGB::DarkOrange, CRGB::DarkOrange, CRGB::Indigo, CRGB::Indigo);
  gPaletteHolyJoly        = getHolyJolyPalette       ( CRGB::Red, CRGB::Red, CRGB::Green, CRGB::Green);
  gPaletteIndependance    = getIndependancePalette   ( CRGB::FireBrick, CRGB::Cornsilk, CRGB::MediumBlue, CRGB::MediumBlue);
  gPaletteFire            = HeatColors_p;

  fill_solid(leds, NUM_LEDS, CRGB(gColorR, gColorG, gColorB)); //Startup LED Lights
  FastLED.show();
  
  wifi_setup();

  gMqttClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  gMqttClient.setCallback(mqtt_callback);

  Serial.print("Ready - IP: ");
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
  // TODO: MEMCPY/MEMSET THIS
  for (int i = 0; i < length; i++) 
    message_buff[i] = payload[i];
    
  message_buff[length] = '\0';

  Serial.println("mqtt_callback: " + String(topic) + " - " + String(message_buff));

  if (String(topic) == MQTT_SUB_SET_POWER) 
  {    
    if (String(message_buff) == "ON") 
    {
      gEnabled = true;
      gMqttClient.publish(MQTT_PUB_SET_POWER, "ON");
    }
    else
    {
      gEnabled = false;
      gMqttClient.publish(MQTT_PUB_SET_POWER, "OFF");        
    }
  }
  else if (String(topic) == MQTT_SUB_SET_EFFECT) 
  {
    gEffect = String(message_buff);    

    // Init some counters
    twinklecounter = 0;
    gStartIndex = 0;

    gMqttClient.publish(MQTT_PUB_SET_EFFECT, gEffect.c_str());    
  }
  else if (String(topic) == MQTT_SUB_SET_BRIGHTNESS) 
  {
    gBrightness = String(message_buff).toInt();
    gMqttClient.publish(MQTT_PUB_SET_BRIGHTNESS, String(gBrightness).c_str());
  }
  else if (String(topic) == MQTT_SUB_SET_COLORR) 
  {
    gColorR = String(message_buff).toInt();
    gMqttClient.publish(MQTT_PUB_SET_COLORR, String(gColorR).c_str());
  }
  else if (String(topic) == MQTT_SUB_SET_COLORG) 
  {
    gColorG = String(message_buff).toInt();
    gMqttClient.publish(MQTT_PUB_SET_COLORG, String(gColorG).c_str());
  }
  else if (String(topic) == MQTT_SUB_SET_COLORB) 
  {
    gColorB = String(message_buff).toInt();
    gMqttClient.publish(MQTT_PUB_SET_COLORB, String(gColorB).c_str());
  }
  else if (String(topic) == MQTT_SUB_ANIMATION_SPEED) 
  {
    gAnimationSpeed = String(message_buff).toInt();
    gMqttClient.publish(MQTT_PUB_ANIMATION_SPEED, String(gAnimationSpeed).c_str());          
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
void update_effect(const String &aEffect)
{
  random16_add_entropy( random8());

  if(true == gEnabled)
  {
    gStartIndex++;
  
    /////////////////////////////////////////  
    //////DrZzs custom effects//////////////
    ///////////////////////////////////////
    if (gEffect == "Christmas") 
    {
      uint8_t beat = beatsin8( gBeatsPerMinute, 64, 255);
      CRGBPalette16 lPalette = bhw2_xmas_gp;
      for( int i = 0; i < NUM_LEDS; i++) 
      {
        leds[i] = ColorFromPalette(lPalette, gHue+(i*2), beat-gHue+(i*10));
      }
    }
    else if (gEffect == "St Patty") 
    {
      uint8_t beat = beatsin8( gBeatsPerMinute, 64, 255);
      CRGBPalette16 lPalette = bhw2_greenman_gp;
      for( int i = 0; i < NUM_LEDS; i++) 
      {
        leds[i] = ColorFromPalette(lPalette, gHue+(i*2), beat-gHue+(i*10));
      }
    }
    else if (gEffect == "Valentine") 
    {
      uint8_t beat = beatsin8( gBeatsPerMinute, 64, 255);
      CRGBPalette16 lPalette = bhw2_redrosey_gp;    
      for( int i = 0; i < NUM_LEDS; i++) 
      {
        leds[i] = ColorFromPalette(lPalette, gHue+(i*2), beat-gHue+(i*10));
      }
    }  
    if (gEffect == "Turkey Day") 
    { 
      uint8_t beat = beatsin8( gBeatsPerMinute, 64, 255);
      CRGBPalette16 lPalette = bhw2_thanks_gp;      
      for( int i = 0; i < NUM_LEDS; i++) 
      {
        leds[i] = ColorFromPalette(lPalette, gHue+(i*2), beat-gHue+(i*10));
      }
    }
    else if (gEffect == "Thanksgiving") 
    {
      fill_palette( leds, NUM_LEDS,
                    gStartIndex, 16, /* higher = narrower stripes */
                    gPaletteThanksgiving, 255, LINEARBLEND);
    }
    else if (gEffect == "USA") 
    {
      uint8_t beat = beatsin8( gBeatsPerMinute, 64, 255);
      CRGBPalette16 lPalette = bhw3_41_gp;      
      for( int i = 0; i < NUM_LEDS; i++) 
      {
        leds[i] = ColorFromPalette(lPalette, gHue+(i*2), beat-gHue+(i*10));
      }
    }
    else if (gEffect == "Independence") 
    {
      fill_palette( leds, NUM_LEDS,
                    gStartIndex, 16, /* higher = narrower stripes */
                    gPaletteIndependance, 255, LINEARBLEND);
    }
    else if (gEffect == "Halloween") 
    {
      uint8_t beat = beatsin8( gBeatsPerMinute, 64, 255);
      CRGBPalette16 lPalette = Orange_to_Purple_gp;          
      for( int i = 0; i < NUM_LEDS; i++) 
      {
        leds[i] = ColorFromPalette(lPalette, gHue+(i*2), beat-gHue+(i*10));
      }
    }
    else if (gEffect == "Go Blue") 
    {
      uint8_t beat = beatsin8( gBeatsPerMinute, 64, 255);
      CRGBPalette16 lPalette = Pills_3_gp;         
      for( int i = 0; i < NUM_LEDS; i++) 
      {
        leds[i] = ColorFromPalette(lPalette, gHue+(i*2), beat-gHue+(i*10));
      }
    }
    else if (gEffect == "Hail") 
    {
      fill_palette( leds, NUM_LEDS,
                    gStartIndex, 16, /* higher = narrower stripes */
                    gPaletteHail, 255, LINEARBLEND);
    }
    else if (gEffect == "Touchdown") 
    {
      idex++;
      if (idex >= NUM_LEDS) 
      {
        idex = 0;
      }
      int idexY = idex;
      int idexB = antipodal_index(idexY);
      int thathue = (thishuehail + 96) % 255;
      leds[idexY] = CHSV(thishuehail, thissat, 255);
      leds[idexB] = CHSV(thathue, thissat, 255);
    }
    else if (gEffect == "Punkin") 
    {
      fill_palette( leds, NUM_LEDS,
                    gStartIndex, 16, /* higher = narrower stripes */
                    gPaletteHalloween, 255, LINEARBLEND);
    }
    else if (gEffect == "Lovey Day") 
    {
      idex++;
      if (idex >= NUM_LEDS) {
        idex = 0;
      }
      int idexR = idex;
      int idexB = antipodal_index(idexR);
      int thathue = (thishueLovey + 244) % 255;
      leds[idexR] = CHSV(thishueLovey, thissat, 255);
      leds[idexB] = CHSV(thathue, thissat, 255);
    }
    else if (gEffect == "Holly Jolly") 
    {
      fill_palette( leds, NUM_LEDS,
                    gStartIndex, 16, /* higher = narrower stripes */
                    gPaletteHolyJoly, 255, LINEARBLEND);
    }
    ///////////////////////////////////////////////
    /////////fastLED & Bruh effects///////////////
    /////////////////////////////////////////////
    else if (gEffect == "Sinelon") 
    {
      fadeToBlackBy( leds, NUM_LEDS, 20);
      int pos = beatsin16(13, 0, NUM_LEDS);
      leds[pos] += CRGB(gColorR, gColorG, gColorB);
    }
    else if (gEffect == "Juggle" ) 
    {
      fadeToBlackBy( leds, NUM_LEDS, 20);
      byte dothue = 0;
      for ( int i = 0; i < 8; i++) 
      {
        leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CRGB(gColorR, gColorG, gColorB);
        dothue += 32;
      }
    }
    else if (gEffect == "Confetti" ) 
    {
      fadeToBlackBy( leds, NUM_LEDS, 10);
      int pos = random16(NUM_LEDS);
      leds[pos] += CRGB(gColorR + random8(64), gColorG, gColorB);
    }
    else if (gEffect == "Rainbow") 
    {
      // FastLED's built-in rainbow generator
      static uint8_t starthue = 0;    
      thishue++;
      fill_rainbow(leds, NUM_LEDS, thishue, deltahue);
    }
    else if (gEffect == "Rainbow with Glitter") 
    {
      // FastLED's built-in rainbow generator with Glitter
      static uint8_t starthue = 0;
      thishue++;
      fill_rainbow(leds, NUM_LEDS, thishue, deltahue);
  
      if( random8() < 80) 
      {
        leds[ random16(NUM_LEDS) ] += CRGB::White;
      }    
    }
    else if (gEffect == "Glitter") 
    {
      fadeToBlackBy( leds, NUM_LEDS, 20);
  
      if( random8() < 80) 
      {
        leds[ random16(NUM_LEDS) ] += CRGB(gColorR, gColorG, gColorB);
      }    
    }
    else if (gEffect == "BPM") 
    {
      CRGBPalette16 palette = PartyColors_p;
      uint8_t beat = beatsin8( gBeatsPerMinute, 64, 255);
      for( int i = 0; i < NUM_LEDS; i++) 
      {
        leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
      }
    }
    else if (gEffect == "Solid") 
    {
      fill_solid(leds, NUM_LEDS, CRGB(gColorR, gColorG, gColorB));
    }
    else if (gEffect == "Twinkle") 
    {
      twinklecounter = twinklecounter + 1;
      if (twinklecounter < 2) 
      {
        //Resets strip if previous animation was running
        FastLED.clear();
        FastLED.show();
      }
      const CRGB lightcolor(8, 7, 1);
      for ( int i = 0; i < NUM_LEDS; i++) {
        if ( !leds[i]) continue; // skip black pixels
        if ( leds[i].r & 1) { // is red odd?
          leds[i] -= lightcolor; // darken if red is odd
        } else {
          leds[i] += lightcolor; // brighten if red is even
        }
      }
      if ( random8() < DENSITY) {
        int j = random16(NUM_LEDS);
        if ( !leds[j] ) leds[j] = lightcolor;
      }
    }
    else if (gEffect == "Dots") 
    {
      uint8_t inner = beatsin8(bpm, NUM_LEDS / 4, NUM_LEDS / 4 * 3);
      uint8_t outer = beatsin8(bpm, 0, NUM_LEDS - 1);
      uint8_t middle = beatsin8(bpm, NUM_LEDS / 3, NUM_LEDS / 3 * 2);
      leds[middle] = CRGB::Purple;
      leds[inner] = CRGB::Blue;
      leds[outer] = CRGB::Aqua;
      nscale8(leds, NUM_LEDS, fadeval);
    }
    else if (gEffect == "Lightning") 
    {
      twinklecounter = twinklecounter + 1;                     //Resets strip if previous animation was running
      Serial.println(twinklecounter);
      if (twinklecounter < 2) {
        FastLED.clear();
        FastLED.show();
      }
      ledstart = random8(NUM_LEDS);           // Determine starting location of flash
      ledlen = random8(NUM_LEDS - ledstart);  // Determine length of flash (not to go beyond NUM_LEDS-1)
      for (int flashCounter = 0; flashCounter < random8(3, flashes); flashCounter++) {
        if (flashCounter == 0) dimmer = 5;    // the brightness of the leader is scaled down by a factor of 5
        else dimmer = random8(1, 3);          // return strokes are brighter than the leader
        fill_solid(leds + ledstart, ledlen, CHSV(255, 0, 255 / dimmer));
        FastLED.show();                       // Show a section of LED's
        delay(random8(4, 10));                // each flash only lasts 4-10 milliseconds
        fill_solid(leds + ledstart, ledlen, CHSV(255, 0, 0)); // Clear the section of LED's
        FastLED.show();
        if (flashCounter == 0) delay (150);   // longer delay until next flash after the leader
        delay(50 + random8(100));             // shorter delay between strokes
      }
      delay(random8(frequency) * 100);        // delay between strikes
    }
    else if (gEffect == "Police One") {                    //POLICE LIGHTS (TWO COLOR SINGLE LED)
      idex++;
      if (idex >= NUM_LEDS) {
        idex = 0;
      }
      int idexR = idex;
      int idexB = antipodal_index(idexR);
      int thathue = (thishuepolice + 160) % 255;
      for (int i = 0; i < NUM_LEDS; i++ ) {
        if (i == idexR) {
          leds[i] = CHSV(thishuepolice, thissat, 255);
        }
        else if (i == idexB) {
          leds[i] = CHSV(thathue, thissat, 255);
        }
        else {
          leds[i] = CHSV(0, 0, 0);
        }
      }
  
    }
    else if (gEffect == "Police All") 
    {                 //POLICE LIGHTS (TWO COLOR SOLID)
      idex++;
      if (idex >= NUM_LEDS) {
        idex = 0;
      }
      int idexR = idex;
      int idexB = antipodal_index(idexR);
      int thathue = (thishuepolice + 160) % 255;
      leds[idexR] = CHSV(thishuepolice, thissat, 255);
      leds[idexB] = CHSV(thathue, thissat, 255);
    }
    else if (gEffect == "Candy Cane") {
      fill_palette( leds, NUM_LEDS,
                    gStartIndex, 16, /* higher = narrower stripes */
                    gPaletteCandyCane, 255, LINEARBLEND);
    }
    else if (gEffect == "Cyclon Rainbow") {                    //Single Dot Down
      static uint8_t hue = 0;
      Serial.print("x");
      // First slide the led in one direction
      for(int i = 0; i < NUM_LEDS; i++) {
        // Set the i'th led to red 
        leds[i] = CHSV(hue++, 255, 255);
        // Show the leds
        FastLED.show(); 
        // now that we've shown the leds, reset the i'th led to black
        // leds[i] = CRGB::Black;
        fadeall();
        // Wait a little bit before we loop around and do it again
        delay(10);
      }
      for(int i = (NUM_LEDS)-1; i >= 0; i--) {
        // Set the i'th led to red 
        leds[i] = CHSV(hue++, 255, 255);
        // Show the leds
        FastLED.show();
        // now that we've shown the leds, reset the i'th led to black
        // leds[i] = CRGB::Black;
        fadeall();
        // Wait a little bit before we loop around and do it again
        delay(10);
      }
    }
    else if (gEffect == "Fire") 
    { 
      // Array of temperature readings at each simulation cell
      static byte heat[NUM_LEDS];
  
      // Step 1.  Cool down every cell a little
      for( int i = 0; i < NUM_LEDS; i++) {
        heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
      }
    
      // Step 2.  Heat from each cell drifts 'up' and diffuses a little
      for( int k= NUM_LEDS - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
      }
      
      // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
      if( random8() < SPARKING ) {
        int y = random8(7);
        heat[y] = qadd8( heat[y], random8(160,255) );
      }
  
      // Step 4.  Map from heat cells to LED colors
      for( int j = 0; j < NUM_LEDS; j++) {
        // Scale the heat value from 0-255 down to 0-240
        // for best results with color palettes.
        byte colorindex = scale8( heat[j], 240);
        CRGB color = ColorFromPalette( gPaletteFire, colorindex);
        int pixelnumber;
        if( gReverseDirection ) {
          pixelnumber = (NUM_LEDS-1) - j;
        } else {
          pixelnumber = j;
        }
        leds[pixelnumber] = color;
      }
    }
    else if (gEffect == "Easter") 
    {
      for (int i = 0; i < NUM_LEDS; i++)                                       // Just ONE loop to fill up the LED array as all of the pixels change.
      {
        uint8_t index = inoise8(i * scale, dist + i * scale) % 255;            // Get a value from the noise function. I'm using both x and y axis.
        leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
      }
      dist += beatsin8(10, 1, 4);                                              // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
      // In some sketches, I've used millis() instead of an incremented counter. Works a treat.
    }
    else if (gEffect == "Ripple") 
    {
      for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(bgcol++, 255, 15);  // Rotate background colour.
      switch (step) {
        case -1:                                                          // Initialize ripple variables.
          center = random(NUM_LEDS);
          colour = random8();
          step = 0;
          break;
        case 0:
          leds[center] = CHSV(colour, 255, 255);                          // Display the first pixel of the ripple.
          step ++;
          break;
        case maxsteps:                                                    // At the end of the ripples.
          step = -1;
          break;
        default:                                                             // Middle of the ripples.
          leds[(center + step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade / step * 2);   // Simple wrap from Marc Miller
          leds[(center - step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade / step * 2);
          step ++;                                                         // Next step.
          break;
      }
    }
  }
  else
  {
    fadeall();
    delay(10);
//    // Fade off leds
//    for ( int i = 0; i < NUM_LEDS; i++) 
//      leds[i].fadeToBlackBy( 8 );
  }

  EVERY_N_MILLISECONDS(10) 
  {
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // FOR NOISE ANIMATION
    gHue++;
  }
  
  EVERY_N_SECONDS(5) 
  {
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 192, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)));
  }

  FastLED.setBrightness(gBrightness);  //EXECUTE EFFECT COLOR
  FastLED.show();
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
  if (!gMqttClient.connected()) 
  {
    wifi_reconnect();
  }
  gMqttClient.loop();

  // Process the effect
  update_effect(gEffect);  

  if (gAnimationSpeed > 0 && gAnimationSpeed < 150) 
  {  
    FastLED.delay(1000 / gAnimationSpeed);
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
void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } } //for CYCLON


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
  while (!gMqttClient.connected()) 
  {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (gMqttClient.connect(MQTT_SENSOR_NAME, MQTT_USER, MQTT_USER_PW)) 
    {
      Serial.println("connected");

      FastLED.clear (); //Turns off startup LEDs after connection is made
      FastLED.show();

      gMqttClient.subscribe(MQTT_SUB_SET_COLORR);
      gMqttClient.subscribe(MQTT_SUB_SET_COLORG);
      gMqttClient.subscribe(MQTT_SUB_SET_COLORB);
      gMqttClient.subscribe(MQTT_SUB_SET_BRIGHTNESS);
      gMqttClient.subscribe(MQTT_SUB_SET_POWER);
      gMqttClient.subscribe(MQTT_SUB_SET_EFFECT);
      gMqttClient.subscribe(MQTT_SUB_ANIMATION_SPEED);

      if(true == gEnabled)
        gMqttClient.publish(MQTT_PUB_SET_POWER, "ON");
      else
        gMqttClient.publish(MQTT_PUB_SET_POWER, "OFF");
      
      gMqttClient.publish(MQTT_PUB_SET_COLORR, String(gColorR).c_str());
      gMqttClient.publish(MQTT_PUB_SET_COLORG, String(gColorG).c_str());
      gMqttClient.publish(MQTT_PUB_SET_COLORB, String(gColorB).c_str());
      gMqttClient.publish(MQTT_PUB_SET_BRIGHTNESS, String(gBrightness).c_str());
      gMqttClient.publish(MQTT_PUB_SET_EFFECT, gEffect.c_str());
      gMqttClient.publish(MQTT_PUB_ANIMATION_SPEED, String(gAnimationSpeed).c_str());      
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
