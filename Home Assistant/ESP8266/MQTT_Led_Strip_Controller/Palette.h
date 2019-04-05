#ifndef PALETTE_HEADER
#define PALETTE_HEADER
// ****************************************************************************
// Module: Palette Header
//
// Description:
//
// Required Packages:
//
// Acknoledgements:
//
// ****************************************************************************

////////////////////////place setup__Palette and __Palettestriped custom functions here - for Candy Cane effects ///////////////// 
///////You can use up to 4 colors and change the pattern of A's AB's B's and BA's as you like//////////////

// **************************************************************************** 
// Routine: 
//
// Description:
//
// Arguments:
//
// **************************************************************************** 
CRGBPalette16 getStripedPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  return CRGBPalette16(A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B);
}

// **************************************************************************** 
// Routine: 
//
// Description:
//
// Arguments:
//
// **************************************************************************** 
CRGBPalette16 getHailPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  return CRGBPalette16(A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B);
}

// **************************************************************************** 
// Routine: 
//
// Description:
//
// Arguments:
//
// **************************************************************************** 
CRGBPalette16 getHolyJolyPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  return CRGBPalette16(A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B);
}

// **************************************************************************** 
// Routine: 
//
// Description:
//
// Arguments:
//
// **************************************************************************** 
CRGBPalette16 getIndependancePalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  return CRGBPalette16(A, A, A, A, A, AB, AB, AB, AB, AB, B, B, B, B, B, B);
}

// **************************************************************************** 
// Routine: 
//
// Description:
//
// Arguments:
//
// **************************************************************************** 
CRGBPalette16 getThanksgivingPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  return CRGBPalette16(A, A, A, A, A, A, A, AB, AB, AB, B, B, B, B, B, B);
}

// **************************************************************************** 
// Routine: 
//
// Description:
//
// Arguments:
//
// **************************************************************************** 
CRGBPalette16 getHalloweenPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  return CRGBPalette16(A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B);
}
////////////////////////////////////////////////////////


///////////////DrZzs Palettes for custom BPM effects//////////////////////////
///////////////Add any custom palettes here//////////////////////////////////

// Gradient palette "bhw2_thanks_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_thanks.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_thanks_gp ) {
    0,   9,  5,  1,
   48,  25,  9,  1,
   76, 137, 27,  1,
   96,  98, 42,  1,
  124, 144, 79,  1,
  153,  98, 42,  1,
  178, 137, 27,  1,
  211,  23,  9,  1,
  255,   9,  5,  1};

// Gradient palette "bhw2_redrosey_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_redrosey.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_redrosey_gp ) {
    0, 103,  1, 10,
   33, 109,  1, 12,
   76, 159,  5, 48,
  119, 175, 55,103,
  127, 175, 55,103,
  178, 159,  5, 48,
  221, 109,  1, 12,
  255, 103,  1, 10};

// Gradient palette "bluered_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/h5/tn/bluered.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE( bluered_gp ) {
    0,   0,  0,255,
  127, 255,255,255,
  255, 255,  0,  0};

// Gradient palette "bhw2_xmas_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_xmas.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 48 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_xmas_gp ) {
    0,   0, 12,  0,
   40,   0, 55,  0,
   66,   1,117,  2,
   77,   1, 84,  1,
   81,   0, 55,  0,
  119,   0, 12,  0,
  153,  42,  0,  0,
  181, 121,  0,  0,
  204, 255, 12,  8,
  224, 121,  0,  0,
  244,  42,  0,  0,
  255,  42,  0,  0};

// Gradient palette "bhw2_xc_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_xc.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_xc_gp ) {
    0,   4,  2,  9,
   58,  16,  0, 47,
  122,  24,  0, 16,
  158, 144,  9,  1,
  183, 179, 45,  1,
  219, 220,114,  2,
  255, 234,237,  1};

// Gradient palette "bhw1_04_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_04.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw1_04_gp ) {
    0, 229,227,  1,
   15, 227,101,  3,
  142,  40,  1, 80,
  198,  17,  1, 79,
  255,   0,  0, 45};

// Gradient palette "bhw4_051_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_051.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

// Gradient palette "fs2006_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/cl/tn/fs2006.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 56 bytes of program space.

DEFINE_GRADIENT_PALETTE( fs2006_gp ) {
    0,   0, 49,  5,
   34,   0, 49,  5,
   34,  79,168, 66,
   62,  79,168, 66,
   62, 252,168, 92,
  103, 252,168, 92,
  103, 234, 81, 29,
  143, 234, 81, 29,
  143, 222, 30,  1,
  184, 222, 30,  1,
  184,  90, 13,  1,
  238,  90, 13,  1,
  238, 210,  1,  1,
  255, 210,  1,  1};


DEFINE_GRADIENT_PALETTE( bhw4_051_gp ) {
    0,   1,  1,  4,
   28,  16, 24, 77,
   66,  35, 87,160,
  101, 125,187,205,
  127, 255,233, 13,
  145, 125,187,205,
  193,  28, 70,144,
  224,  14, 19, 62,
  255,   1,  1,  4};

// Gradient palette "blue_g2_5_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/go2/webtwo/tn/blue-g2-5.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( blue_g2_5_gp ) {
    0,   2,  6, 63,
  127,   2,  9, 67,
  255,   255, 255, 115,
  255,   255, 255, 0};

// Gradient palette "bhw3_41_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw3/tn/bhw3_41.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw3_41_gp ) {
    0,   0,  0, 45,
   71,   7, 12,255,
   76,  75, 91,255,
   76, 255,255,255,
   81, 255,255,255,
  178, 255,255,255,
  179, 255, 55, 45,
  196, 255,  0,  0,
  255,  42,  0,  0};

DEFINE_GRADIENT_PALETTE( test_gp ) {
    0,  255,  0,  0, // Red
// 32,  171, 85,  0, // Orange
// 64,  171,171,  0, // Yellow
// 96,    0,255,  0, // Green
//128,    0,171, 85, // Aqua
  160,    0,  0,255, // Blue
//192,   85,  0,171, // Purple
//224,  171,  0, 85, // Pink
//255,  255,  0,  0};// and back to Red
};  

// Gradient palette "bhw2_greenman_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_greenman.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_greenman_gp ) {
    0,   1, 22,  1,
  130,   1,168,  2,
  255,   1, 22,  1};

// Gradient palette "Pills_3_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds/icons/tn/Pills-3.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE( Pills_3_gp ) {
    0,   4, 12,122,
  127,  55, 58, 50,
  255, 192,147, 11};

// Gradient palette "Orange_to_Purple_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds/icons/tn/Orange-to-Purple.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE( Orange_to_Purple_gp ) {
    0, 208, 50,  1,
  127, 146, 27, 45,
  255,  97, 12,178};

#endif // PALETTE_HEADER
