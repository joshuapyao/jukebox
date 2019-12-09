#include "Adafruit_VL53L0X.h"
#include <Adafruit_NeoPixel.h>

#define PIN      6
#define N_LEDS 72

char val;
int trigPins[] = {7, 10, 2, 4};
int echoPins[] = {8, 11, 3, 5};
int pinCount = 4;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int LEdA[] = {6, 7, 8, 9, 10, 11, 12, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51}; //19  
int LEdB[] = {21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35}; //15 
int LEdC[] = {14, 15, 16, 17, 18, 19, 20, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48}; //19 
int LEdD[] = {0, 1, 2, 3, 4, 5, 64, 65, 66, 67, 68, 69, 70, 71}; //14 


//long duration;
float distance1;
float distance2;
float distance3;
float distance4;
float distanceFromA;

float min = 2.0;
float max = 27.94 - min;

boolean shouldPulse;

void setup() {

  strip.begin();
  shouldPulse = true;
  // put your setup code here, to run once:
  for (int i=0; i< pinCount; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }

  Serial.begin(9600);
  
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  //establishContact();

  //adafruit code
    Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
//Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
  
}


static void chase(uint32_t c) {
  Serial.print("hello LED");
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      strip.setPixelColor(i, c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
     delay(25);
  }
}

// 0 to 255
void brightenAll() {
  uint16_t j;

  for (j = 5; j < 255; j++) {
    for (uint16_t i = 0; i < 19; i++) 
      strip.setPixelColor(LEdA[i], j, 0, j);
    for (uint16_t i = 0; i < 15; i++) 
      strip.setPixelColor(LEdB[i], j, 0, j);
    for (uint16_t i = 0; i < 19; i++) 
      strip.setPixelColor(LEdC[i], 0, 0, j);
    for (uint16_t i = 0; i < 14; i++) 
      strip.setPixelColor(LEdD[i], j, 0, 0);  
    //}
    strip.show();
    //delay(10);
  }
  //delay(1500);
}

void brightenA() {
  uint16_t j;

  //for (j = 5; j < 255; j++) {
    for (uint16_t i = 0; i < 19; i++) 
      strip.setPixelColor(LEdA[i], 255, 0, 255);
    strip.show();
    //delay(10);
  //}
  //delay(1500);
}

void brightenB() {
  uint16_t j;

  //for (j = 5; j < 255; j++) {
    for (uint16_t i = 0; i < 15; i++) 
      strip.setPixelColor(LEdB[i], 255, 0, 255);
    strip.show();
    //delay(10);
  //}
  //delay(1500);
}

void brightenC() {
  uint16_t j;

  //for (j = 5; j < 255; j++) {
    for (uint16_t i = 0; i < 19; i++) 
      strip.setPixelColor(LEdC[i], 255, 0, 255);
    strip.show();
    //delay(10);
  //}
  //delay(1500);
}

void brightenD() {
  uint16_t j;

  //for (j = 5; j < 255; j++) {
    for (uint16_t i = 0; i < 14; i++) 
      strip.setPixelColor(LEdD[i], 255, 0, 255);
    strip.show();
    //delay(10);
  //}
  //delay(1500);
}

// 255 to 0
void darkenAll() {
  //Serial.begin(9600);
  uint16_t j;

  for (j = 255; j > 5; j--) {
     for (uint16_t i = 0; i < 19; i++) 
      strip.setPixelColor(LEdA[i], j, 0, j);
    for (uint16_t i = 0; i < 15; i++) 
      strip.setPixelColor(LEdB[i],j, 0, j);
    for (uint16_t i = 0; i < 19; i++) 
      strip.setPixelColor(LEdC[i], 0, 0, j);
    for (uint16_t i = 0; i < 14; i++) 
      strip.setPixelColor(LEdD[i], j, 0, 0); 
    
    strip.show();
    //delay(10);
    //Serial.println(j);
  }
 // delay(1500);
}

void darkenA() {
  //Serial.begin(9600);
  uint16_t j;

  //for (j = 255; j > 5; j--) {
     for (uint16_t i = 0; i < 19; i++) 
      strip.setPixelColor(LEdA[i], 0, 0, 0);
    
    strip.show();
    //delay(10);
    //Serial.println(j);
 // }
 // delay(1500);
}

void darkenB() {
  //Serial.begin(9600);
  uint16_t j;

  //for (j = 255; j > 5; j--) {
     for (uint16_t i = 0; i < 15; i++) 
      strip.setPixelColor(LEdB[i], 0, 0, 0);
    
    strip.show();
    //delay(10);
    //Serial.println(j);
 // }
 // delay(1500);
}

void darkenC() {
  //Serial.begin(9600);
  uint16_t j;

  //for (j = 255; j > 5; j--) {
     for (uint16_t i = 0; i < 19; i++) 
      strip.setPixelColor(LEdC[i], 0, 0, 0);
    
    strip.show();
    //delay(10);
    //Serial.println(j);
 // }
 // delay(1500);
}

void darkenD() {
  //Serial.begin(9600);
  uint16_t j;

  //for (j = 255; j > 5; j--) {
     for (uint16_t i = 0; i < 14; i++) 
      strip.setPixelColor(LEdD[i], 0, 0, 0);
    
    strip.show();
    //delay(10);
    //Serial.println(j);
 // }
 // delay(1500);
}


void loop() {
  shouldPulse = true;
  for (int i=0; i< pinCount; i++) {
    digitalWrite(trigPins[i], LOW);
    delayMicroseconds(2);
    digitalWrite(trigPins[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[i], LOW);

//    distanceFromA = pulseIn(3, HIGH)*0.034/2;
//    Serial.print("New C");
//    Serial.println(distanceFromA);

   
    if (i == 0) {
       distance1 = pulseIn(echoPins[0], HIGH)*0.034/2;
       Serial.print('A');
        Serial.println(distance1);
        if(distance1 <= max && distance1 >= min){
          shouldPulse = false;
          brightenA();
        }
        else{
          darkenA();
        }
    } else if (i == 1) {

      distance2 = pulseIn(echoPins[1], HIGH)*0.034/2;
      Serial.print('B');
      Serial.println(distance2);
      
      if(distance2 <= max && distance2 >= min){
       //strip.setPixelColor(LEdB[i], 255, 0, 255);
       shouldPulse = false;
       brightenB();
      }
      else{
        darkenB();
      }
   
        
    } else if (i == 2) {
      distance3 = pulseIn(echoPins[2], HIGH)*0.034/2;
      Serial.print('C');
       Serial.println(distance3);
       if(distance3 <= max && distance3 >= min){
          shouldPulse = false;
          brightenC();
        }
      else{
        darkenC();
      }
    } else if (i == 3) {
      distance4 = pulseIn(echoPins[3], HIGH)*0.034/2;
      Serial.print('D');
       Serial.println(distance4);
       if(distance4 <= max && distance4 >= min){
          shouldPulse = false;
          brightenD();
        }
        else{
          darkenD();
        }
    }

    if( !(distance1 <= max && distance1 >= min) && !(distance2 <= max && distance2 >= min) && !(distance3 <= max && distance3 >= min) && !(distance4 <= max && distance4 >= min)){
        shouldPulse = true;
    }
    else
    {
    shouldPulse = false;
    }

    if(i == pinCount - 1 && shouldPulse){
      brightenAll();
      darkenAll();
    }
//    else{
//     Serial.print("False");
//    }
    
    //Serial.println(distance);
    //delay(50);

    // chase(strip.Color(255, 0, 0)); // Red
    //chase(strip.Color(0, 0, 255)); // Blue

  }


  //adafruit code
   VL53L0X_RangingMeasurementData_t measure;
    
  //Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    float  dist = measure.RangeMilliMeter/10;
    Serial.print("E"); Serial.println(dist);
  } else {
    Serial.println(" out of range ");
  }
    
  delay(100);
  

}
