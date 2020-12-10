 
// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define PIN 0
#define MAX_ANIMATION_TYPE 23

#define MAX_ANIMATION_STEP 31

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, PIN);
 

uint8_t  animationType   = 0; // Current animation effect
uint8_t  animationStepOffset, animationStepDecay;
  
uint32_t color  = 0x0000FF;

uint32_t colorsArray[] = {0xFF0000, 0xFFFF00 , 0x00FFFF, 0xFF00FF, 0x00FF00 , 0x0000FF };

uint8_t prevButtonState = 0;

uint32_t lastStateChangeTime;
 

//uint8_t animationStepTime = 50;
uint8_t animationStep;
uint32_t timeSinceAnimationStepIncrement;


  
void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.setBrightness(85); // 1/3 brightness
 

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

      animationStepOffset = 50;
      animationStepDecay = 0;

         colorIndex = animationType % 6 ;
         color = colorsArray[colorIndex];
   
     
       uint32_t c = 0;
       
     for(i=0; i<16; i++) {

      
       
      if(((animationStep + i) & 7) < 2) c = color; // 4 pixels on...
 
       c >>= 3;                 // Color fader 
      
      pixels.setPixelColor(   i, c); // First eye
      pixels.setPixelColor(31-i, c); // Second eye (flipped)
     }
     
   
     
 }

 
 else if(animationType <= 11){
    // Solid color wheels 

      animationStepOffset = 50;
      animationStepDecay = 0;


         colorIndex = animationType % 6;
         color = colorsArray[colorIndex]; 
      
      for(i=0; i<16; i++) {
        uint32_t c = 0;
        if(((animationStep+i) & 7) < 2) c = color; // 4 pixels on...
   
         //c >>= 8;
        pixels.setPixelColor(   i, c); // First eye
        pixels.setPixelColor(31-i, c); // Second eye (flipped)
      }
       
    
 }


 else if(animationType <= 17){


  // Solid color wheels with changing step times 


      animationStepOffset = 0;
      animationStepDecay = 10;


         colorIndex = animationType % 6;
         color = colorsArray[colorIndex]; 
      
      for(i=0; i<16; i++) {
        uint32_t c = 0;
        if(((animationStep+i) & 7) < 2) c = color; // 4 pixels on...
   
         //c >>= 8;
        pixels.setPixelColor(   i, c); // First eye
        pixels.setPixelColor(31-i, c); // Second eye (flipped)
      }
       ;
   
 }


   else if(animationType <= 23){
   
  // Rainbow wheels   

      animationStepOffset = 35 + (animationType % 6) * 5;
      animationStepDecay = 2 + (animationType % 6) * 2 ;
 

        // colorIndex = animationType % 6 ;+
         color = 0xc794b7;
   
      
        color = hueToColorHex(0.1) ;
     

     for(i=0; i< (animationStep % 16 ) ; i++) { 
       // color ^= (animationType % 6); 
       color <<= 2;                 // Color fader 
     }
        
     for(i=0; i<16; i++) {  
      uint32_t c = 0;
      if(((animationStep + i)  & 7  ) < 1 + (animationType % 6) ){
        
           c = color; // 4 pixels on... 
        
        }
      
      pixels.setPixelColor(   i, c); // First eye
      pixels.setPixelColor(31-i, c); // Second eye (flipped)
     }
       
   
   }
 
  
   pixels.show();
   delay(5); 
}

void handleButtonPressed(){
  
   if(lastStateChangeTime < millis() - 50){

       lastStateChangeTime = millis();
  
       animationType = animationType+1  ;
       animationStep = 0; //restart 
    
        if(animationType > MAX_ANIMATION_TYPE ){
          animationType = 0; 
        }
        
    }


}



int getAnimationStepTime(){
  

  return animationStepOffset + ((animationStep % 16) * animationStepDecay);
  
}

uint32_t hueToColorHex(float hue)
{  
    
   byte *r,*g,*b;

   HSV_to_RGB(hue,100.0,100.0,r,g,b);
 

    return colorBytesToHex(&r,&b,&g);

    
}

uint32_t colorBytesToHex(byte r,byte g,byte b){
  
  return  (r << 16 ) | (g << 8) | b;   
  
}



void HSV_to_RGB(float h, float s, float v, byte *r, byte *g, byte *b)
{
  int i;
  float f,p,q,t;
  
  h = max(0.0, min(360.0, h));
  s = max(0.0, min(100.0, s));
  v = max(0.0, min(100.0, v));
  
  s /= 100;
  v /= 100;

   s = 1.0;
   v = 1.0;
   
  if(s == 0) {
    // Achromatic (grey)
    *r = *g = *b = round(v*255);
    return;
  }
  
  

  h /= 60; // sector 0 to 5
  i = floor(h);
  f = h - i; // factorial part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));
  switch(i) {
    case 0:
      *r = round(255*v);
      *g = round(255*t);
      *b = round(255*p);
      break;
    case 1:
      *r = round(255*q);
      *g = round(255*v);
      *b = round(255*p);
      break;
    case 2:
      *r = round(255*p);
      *g = round(255*v);
      *b = round(255*t);
      break;
    case 3:
      *r = round(255*p);
      *g = round(255*q);
      *b = round(255*v);
      break;
    case 4:
      *r = round(255*t);
      *g = round(255*p);
      *b = round(255*v);
      break;
    default: // case 5:
      *r = round(255*v);
      *g = round(255*p);
      *b = round(255*q);
    }
}
