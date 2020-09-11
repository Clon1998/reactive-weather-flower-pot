#pragma once
#include <FastLED.h>
// Gradient palette "temperature_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/arendal/tn/temperature.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 144 bytes of program space.

// DEFINE_GRADIENT_PALETTE(temperature_gp){
//     0, 1, 27, 105,// -25--
//     14, 1, 27, 105,//Freeeeeeezing Cold?- 20?
//     28, 1, 40, 127,
//     42, 1, 70, 168,// Very Cold -10 
//     56, 1, 92, 197,
//     70, 1, 119, 221,// Cold -5
//     84, 3, 130, 151,
//     99, 23, 156, 149,// 0
//     113, 67, 182, 112,
//     127, 121, 201, 52,
//     141, 142, 203, 11,// Chill 15
//     155, 224, 223, 1,
//     170, 252, 187, 2,
//     184, 247, 147, 1,// Hot 27+
//     198, 237, 87, 1,
//     212, 229, 43, 1,
//     226, 171, 2, 2,// Very Hot // 36+
//     240, 171, 2, 2,
//     254, 80, 3, 3};// Very Hot 42+
// Gradient palette "temperature_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/jjg/misc/tn/temperature.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 184 bytes of program space.

DEFINE_GRADIENT_PALETTE( temperature_gp ) {
    0,   1,  1,106,
   11,   1,  6,137,
   22,   2, 13,140,
   33,   4, 22,144,
   44,   4, 29,135,
   55,   8, 40,151,
   66,  12, 53,156,
   77,  16, 69,164,
   88,  21, 85,170,
   99,  40,125,203,
  110,  82,175,255,
  121, 128,201,111,
  133, 103,189, 89,
  144,  97,175, 64,
  155, 133,161, 35,
  166, 171,149, 19,
  177, 177,131, 14,
  188, 167, 96, 11,
  199, 155, 74,  8,
  210, 152, 60,  7,
  221, 142, 42,  6,
  232, 139, 31,  4,
  243, 133, 14,  2,
  255, 133, 14,  2};
