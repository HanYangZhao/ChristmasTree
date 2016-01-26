// This #include statement was automatically added by the Particle IDE.
#include "SparkIntervalTimer/SparkIntervalTimer.h"



// This #include statement was automatically added by the Particle IDE.
#include "neopixel/neopixel.h"


//#define BLYNK_DEBUG // Uncomment this to see debug prints
#define BLYNK_PRINT Serial
#include "blynk/BlynkSimpleParticle.h"
#define PIXEL_COUNT 360
#define PIXEL_PIN D1
#define PIXEL_TYPE WS2812B

SYSTEM_MODE(AUTOMATIC);
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "f3671d8e84b04b238f947fbfbfb121c9";
uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;
bool manual_spiral = false;
bool manual_area = false;
int current_pos = 0;
int spiral_pos = 0;
bool spiral_down = true;
bool spiral_up = false;
bool spiral = false;
bool rainbow_color = false;



int r_counter = 0;
int temp_r;
int temp_b;
int temp_g;

bool rainbow_switch = false;

int interval = 175;

float brightness = 1.0f;

bool ran_color = false;
IntervalTimer myTimer;		// 3 for the Core
IntervalTimer myTimer2;
IntervalTimer myTimer3;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN,PIXEL_TYPE);
void setup()
{

    Blynk.begin(auth);
    Spark.connect();
    strip.begin();
    led_clear();
}

// Attach a Button widget (mode: Push) to the Virtual pin 1 - and send sweet tweets!
BLYNK_WRITE(V1) {
    if (param.asInt() == 1) { // On button down...
        //Blynk.begin(auth);
        //strip.show();
        led_clear();
        //theaterChase(strip.Color(127, 127, 127), 50); // White
        //rainbowCycle(10);
        //theaterChaseRainbow(50); 
        //strip.show();
    }
    else {
        led_clear();
    }
   
}

// Attach a Slider widget to the Virtual pin 2 - and control the built-in RGB led!
BLYNK_WRITE(V2) {
    brightness =  float (param.asInt() / 100.0f);
}

//strand control
BLYNK_WRITE(V3){
    //led_clear();
    if(manual_spiral && !manual_area){
        if(param.asInt() > current_pos){
            for(int i = 0 ; i <= param.asInt() ; i++){
                strip.setPixelColor(i, strip.Color(red * brightness, green * brightness , blue * brightness));
            }
            current_pos = param.asInt();
            strip.show();
        }
        
        else{
            for(int i = strip.numPixels() ; i >= param.asInt() ; i--){
                strip.setPixelColor(i, strip.Color(0,0,0));
            }
            strip.show();
            current_pos = param.asInt();
        }  
    }
    
    else if(manual_area && !manual_spiral){
        int offset = 5;
        if(param.asInt() != current_pos){
            for (int i = param.asInt() - offset ; i <= param.asInt() + offset ; i++){
                strip.setPixelColor(i, strip.Color(red * brightness, green * brightness , blue * brightness));
            }
            
            for(int i = strip.numPixels() ; i > param.asInt() + offset ; i--){
                strip.setPixelColor(i, strip.Color(0,0,0));
            }
            
            for(int i = 0 ; i < param.asInt() - offset ; i++){
                strip.setPixelColor(i, strip.Color(0,0,0));
            }
            
            current_pos = param.asInt();
            strip.show();
        }
    }
}


BLYNK_WRITE(V4){
   red = param.asInt();
}

BLYNK_WRITE(V5){
    green = param.asInt();
}

BLYNK_WRITE(V6){
   blue = param.asInt();
}

//manual control
BLYNK_WRITE(V7){
    if(param.asInt() == 1){
        led_clear();
        manual_spiral = true;
    }
}

//spiral
BLYNK_WRITE(V8){
    if(param.asInt() == 1){
        led_clear();
        myTimer.begin(spiralLED, 10 , hmSec);
    }
    else if (param.asInt() == 0){
        led_clear();
    }
}

//random pulse
BLYNK_WRITE(V9){
    
    if(param.asInt() == 1){
        led_clear();
        myTimer2.begin(r_LED,80,hmSec);
        
    }
    
    else if(param.asInt() == 0){
        led_clear();
    }
}

//reset
BLYNK_WRITE(V10){
    led_clear();
}

//rainbow switch
// BLYNK_WRITE(V11){
//     if(param.asInt() == 1)
//         rainbow_color = true;
//     else if(param.asInt() == 0)
//         rainbow_color = false;
// }

//random color
BLYNK_WRITE(V11){
    if(param.asInt() == 1){
        ran_color = true;
    }

    else if (param.asInt() == 0){
        ran_color = false;
    }
}

BLYNK_WRITE(V12){
    if(param.asInt() == 1){
        led_clear();
        rainbow();
    }
    
    else if(param.asInt() == 0 ){
        led_clear();
    }
}

BLYNK_WRITE(V13){
    if(param.asInt() == 1){
        led_clear();
        manual_area = true;
        manual_spiral = false;
    }
    
    else if(param.asInt() == 0){
        led_clear();
    }
}

BLYNK_WRITE(V14){
    if(param.asInt() == 1){
        led_clear();
        myTimer.begin(ran_blink,interval,hmSec);
    }
    else if(param.asInt() == 0){
        myTimer.end();
        ran_blink();
    }
}

BLYNK_WRITE(V15){
    interval = param.asInt();
}


void loop()
{
    Blynk.run();
}

// *** Utility functions

void ran_blink(void){
    int offset = 5;
    int led = random(300);
        
    for (int i = led - offset ; i <= led + offset ; i++){
        strip.setPixelColor(i, strip.Color(red * brightness, green * brightness , blue * brightness));
    }
            
    for(int i = strip.numPixels() ; i > led + offset ; i--){
        strip.setPixelColor(i, strip.Color(0,0,0));
    }
            
    for(int i = 0 ; i < led - offset ; i++){
        strip.setPixelColor(i, strip.Color(0,0,0));
    }
    strip.show();
        
}


void spiralLED( void ){

    if(ran_color && spiral_pos == 1){
        temp_r = random(255);
        temp_b = random(255);
        temp_g = random(255);
    }
    else if(ran_color && spiral_pos != 1){
        temp_r = temp_r;
        temp_b = temp_b;
        temp_g = temp_g;
    }
    else {
        temp_r = red;
        temp_b = blue;
        temp_g = green;
    }
    if(spiral_down){

        for(int i = 0 ; i <= spiral_pos ; i++){
                strip.setPixelColor(i,strip.Color(temp_r * brightness, temp_g * brightness, temp_b * brightness));
        }
        strip.show();
        spiral_pos++;
        if( spiral_pos > strip.numPixels() ){
            spiral_down = false;
            spiral_up = true;
        }
    }
    
    if(spiral_up) {
        
        for(int i = strip.numPixels() ; i >= spiral_pos ; i--){
            strip.setPixelColor(i, strip.Color(0,0,0));
        }
        strip.show();
        spiral_pos--;
        if( spiral_pos == 0){
            spiral_up = false;
            spiral_down = true;
        }
    }
}

void led_clear(){
    for(int i = 0 ; i < strip.numPixels() ; i++){
        strip.setPixelColor(i,strip.Color(0,0,0));
    }
    myTimer.end();
    myTimer3.end();
    myTimer2.end();
    strip.show();
}


void r_LED(void){

    if(r_counter == 200){
        for(int i = 0 ; i < strip.numPixels() ; i++){
            strip.setPixelColor(i,strip.Color(0,0,0));
        }
        strip.show();
        r_counter = 0;
    }
    
        
    if(r_counter <= 100){
        if(ran_color && r_counter < 1){
            temp_r = random(255);
            temp_b = random(255);
            temp_g = random(255);
        }
        else if(ran_color && r_counter >= 1) {
            temp_r = temp_r;
            temp_b = temp_b;
            temp_g = temp_g;
        }
        else {
            temp_r = red;
            temp_b = blue;
            temp_g = green;
        }
        

        for (int i = 0 ; i < 10 ; i++){
            int led = random(0,360);
            strip.setPixelColor(led,strip.Color(temp_r * brightness ,temp_g * brightness ,temp_b * brightness));
        }
        strip.show();
        r_counter++;
    }
    
    else if(r_counter > 100){

        for(int i = 0 ; i < 20 ; i++){
            int led = random(0,360);
            strip.setPixelColor(led,strip.Color(0, 0, 0 ));
        }
        strip.show();
        r_counter++;
    }
}

void HsvToRgb(double h, double s, double v, byte rgb[]) {
    double r, g, b;

    int i = int(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch(i % 6){
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    rgb[0] = r * 255;
    rgb[1] = g * 255;
    rgb[2] = b * 255;
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}



// Slightly different, this makes the rainbow equally distributed throughout
void rainbow() {
  uint16_t i, j;
  //for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) ) & 255));
  //  }
    strip.show();
    //delay(wait);
    //myTimer.end();
  }
}

//Theatre-style crawling lights.
void theaterChaseWhite() {
  uint32_t previous_millis = 0;
  uint32_t current_millis = 0;
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, strip.Color(175,175,175));    //turn every third pixel on
      }
      strip.show();
      while(current_millis - previous_millis < 5000){
          previous_millis = current_millis;
          current_millis = millis();
      }
     // myTimer.end();

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}