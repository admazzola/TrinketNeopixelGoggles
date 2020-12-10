 
// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define PIN 0
#define MAX_ANIMATION_TYPE 17

#define MAX_ANIMATION_STEP 31

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, PIN);
 

uint8_t  animationType   = 0, // Current animation effect
         offset = 0; // Position of spinny eyes
uint32_t color  = 0x0000FF;

uint32_t colorsArray[] = {0xFF0000, 0xFFFF00 , 0x00FFFF, 0xFF00FF, 0x00FF00 , 0x0000FF };

uint8_t prevButtonState = 0;

uint32_t lastStateChangeTime;
uint32_t prevTime;

//uint8_t animationStepTime = 50;
uint8_t animationStep;
uint32_t timeSinceAnimationStepIncrement;

void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.setBrightness(85); // 1/3 brightness
  prevTime = millis();

  pinMode(1, INPUT);

  
  
}

void loop() {
  uint8_t  i, j;
   
   
  uint8_t  colorIndex;

  uint8_t  buttonState;

  buttonState = digitalRead(1); 
  if(buttonState == HIGH && buttonState != prevButtonState){
    handleButtonPressed();
  }
  prevButtonState = buttonState;
  
 

 if(timeSinceAnimationStepIncrement < millis() - getAnimationStepTime()  ){
    animationStep++;
    
    
    timeSinceAnimationStepIncrement = millis();
 }

 


 if(animationType <= 5){
    // Colorfade wheels 

         colorIndex = animationType % 6 ;
         color = colorsArray[colorIndex];
   
      uint32_t c = 0;
      
       
     for(i=0; i<16; i++) {
      if(((animationStep + i) & 7) < 2) c = color; // 4 pixels on...
 
       c >>= 3;                 // Color fader 
      
      pixels.setPixelColor(   i, c); // First eye
      pixels.setPixelColor(31-i, c); // Second eye (flipped)
     }
     
    pixels.show();
   
     
 }

 
 else if(animationType <= 11){
    // Solid color wheels 

         colorIndex = animationType % 6;
         color = colorsArray[colorIndex]; 
      
      for(i=0; i<16; i++) {
        uint32_t c = 0;
        if(((animationStep+i) & 7) < 2) c = color; // 4 pixels on...
   
         //c >>= 8;
        pixels.setPixelColor(   i, c); // First eye
        pixels.setPixelColor(31-i, c); // Second eye (flipped)
      }
      
    pixels.show();
    
 }


 else if(animationType <= 17){


  // Solid color wheels 

         colorIndex = animationType % 6;
         color = colorsArray[colorIndex]; 
      
      for(i=0; i<16; i++) {
        uint32_t c = 0;
        if(((animationStep+i) & 7) < 2) c = color; // 4 pixels on...
   
         //c >>= 8;
        pixels.setPixelColor(   i, c); // First eye
        pixels.setPixelColor(31-i, c); // Second eye (flipped)
      }
      
    pixels.show();
   
 }

 
 
   delay(5); 
}

void handleButtonPressed(){
  
   if(lastStateChangeTime < millis() - 50){

       lastStateChangeTime = millis();
  
       animationType = animationType+1  ;
    
        if(animationType > MAX_ANIMATION_TYPE ){
        animationType = 0;
        }
        
    }


}


int getAnimationStepTime(){
  
  if(animationType > 11 && animationType <= 17){
    return ((animationStep % 16) * 10);
  }
    
  return 50;
  
}
