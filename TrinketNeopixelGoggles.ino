 
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
    // Colorfade wheels - not working right 

      animationStepOffset = 50;
      animationStepDecay = 0;

         colorIndex = animationType % 6 ;
         color = colorsArray[colorIndex];
   
     
       uint32_t c = 0;
       
    
     
        
     for(i=0; i<16; i++) {
      uint32_t c = 0;
      if(((animationStep + i)  & 7  ) < 1 + (animationType % 6) ){
            int subMode = (animationType % 6) ;
            
            int hue =   subMode * 60 + i*2 ;

           color = hueToColorHex(  hue    ) ;
           
           c = color; // 4 pixels on... 
        
        }
      
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

      animationStepOffset = 35 + (animationType % 6) * 1;
      animationStepDecay = 2 + (animationType % 6) * 1 ;
 

        // colorIndex = animationType % 6 ;+
         //color = 0xc794b7;
   
         
        
     for(i=0; i<16; i++) {
      uint32_t c = 0;
      if(((animationStep + i)  & 7  ) < 1 + (animationType % 6) ){

            int hue = animationStep + i*2  ;

           color = hueToColorHex(  hue    ) ;
           
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

//this is working now 
uint32_t hueToColorHex(int hue)
{   
    
  // byte *r,*g,*b; //declare pointers to bytes 
  

     uint8_t rgb_colors[3]; 

      
   getRGB(hue, 225, 125, rgb_colors);
 
  

   return colorBytesToHex(rgb_colors[0],rgb_colors[1],rgb_colors[2]); //pass the values 

    
}

//this works now when the ints are big enough to be shifted 
uint32_t colorBytesToHex(uint32_t r,uint32_t g,uint32_t b){
   
 
    return  (r << 16  | (g << 8) | b  ); 
  
}



void getRGB(uint8_t hue, uint8_t sat, uint8_t val, uint8_t colors[3]) { 
  /* convert hue, saturation and brightness ( HSB/HSV ) to RGB
     The dim_curve is used only on brightness/value and on saturation (inverted).
     This looks the most natural.      
  */

 // val = dim_curve[val];
  //sat = 255-dim_curve[255-sat];

  hue =   hue % 255;
  
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t base;

  if (sat == 0) { // Acromatic color (gray). Hue doesn't mind.
    colors[0]=val;
    colors[1]=val;
    colors[2]=val;  
  } else  { 

    base = ((255 - sat) * val)>>8;

    switch(hue/60) {
    case 0:
        r = val;
        g = (((val-base)*hue)/60)+base;
        b = base;
    break;

    case 1:
        r = (((val-base)*(60-(hue%60)))/60)+base;
        g = val;
        b = base;
    break;

    case 2:
        r = base;
        g = val;
        b = (((val-base)*(hue%60))/60)+base;
    break;

    case 3:
        r = base;
        g = (((val-base)*(60-(hue%60)))/60)+base;
        b = val;
    break;

    case 4:
        r = (((val-base)*(hue%60))/60)+base;
        g = base;
        b = val;
    break;

    case 5:
        r = val;
        g = base;
        b = (((val-base)*(60-(hue%60)))/60)+base;
    break;
    }

    colors[0]=r;
    colors[1]=g;
    colors[2]=b; 
  }   
}
