#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>
// do NOT include the standard NeoPixel library

#define NEOPIX_PIN    A1
#define NUM_PIXELS    20
#define NUM_MODES     5

// use Adafruit_CPlay_NeoPixel to create a separate external NeoPixel strip
Adafruit_CPlay_NeoPixel strip = Adafruit_CPlay_NeoPixel(NUM_PIXELS, NEOPIX_PIN, NEO_GRB + NEO_KHZ800);
bool lightsaberEnabled = false;
bool previousPowerButtonState = false;
bool bladeExtended = false;
//bool modeButtonPressed = false;


int colorMode;
byte bladeRedValue;
byte bladeGreenValue;
byte bladeBlueValue;
uint32_t bladeColor;
uint32_t flashColor;


void setup() {
  // initialize the Circuit Playground as usual
  // this will initialize the onboard NeoPixels as well
  CircuitPlayground.begin();
  Serial.begin(9600);
  // initialize external NeoPixel strip separately
  strip.begin();
  CircuitPlayground.clearPixels();
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_2_G);
  CircuitPlayground.playTone(262, 100);
  CircuitPlayground.playTone(392, 100);
  CircuitPlayground.playTone(1047, 100);
  updateColors();
}

void loop() {
  // ==== HANDLE BUTTON PRESSES ====
  handleButtons();

  // ==== RUN LIGHTSABER FX =====
  runLightsaber();

  
  /*
  // for the external NeoPixels, must use the Adafruit_CPlay_NeoPixel functions directly
  strip.fill(0xFF00FF);
  strip.show();

  delay(1000);

  // some functions are the same
  for(int i=0; i<15; i+=5){
    strip.setPixelColor(0+i, 255,   0,   0);
    strip.setPixelColor(1+i, 128, 128,   0);
    strip.setPixelColor(2+i,   0, 255,   0);
    strip.setPixelColor(3+i,   0, 128, 128);
    strip.setPixelColor(4+i,   0,   0, 255);
  }
  // but for the external strip, must call show()
  strip.show();

  delay(5000);
  */
}

void handleButtons(){
  bool powerButtonPressed = CircuitPlayground.leftButton();
  bool modeButtonPressed = CircuitPlayground.rightButton();
  if (powerButtonPressed && !previousPowerButtonState){
    previousPowerButtonState = true;
    if(lightsaberEnabled){
      Serial.println("Powering Down");
      lightsaberEnabled = false;
    }else{
      Serial.println("Powering On");
      lightsaberEnabled = true;
    }
    delay(50);
  }else if(!powerButtonPressed && previousPowerButtonState){
    previousPowerButtonState = false;
    delay(50);
  }
 if (modeButtonPressed){
  colorMode = (colorMode + 1)%NUM_MODES;
  updateColors();
  delay(100);
 }
}
void updateColors(){
  switch (colorMode){
    case 0:
      bladeRedValue = 0;
      bladeGreenValue = 255;
      bladeBlueValue = 0;
      bladeColor = strip.Color(bladeRedValue,bladeGreenValue,bladeBlueValue);
      flashColor = strip.Color(255,0,0);
      break;
    case 1:
      bladeRedValue = 0;
      bladeGreenValue = 204;
      bladeBlueValue = 255;
      bladeColor = strip.Color(bladeRedValue,bladeGreenValue,bladeBlueValue);
      flashColor = strip.Color(255,80,0);
      break;
    case 2:
      bladeRedValue = 255;
      bladeGreenValue = 0;
      bladeBlueValue = 255;
      bladeColor = strip.Color(bladeRedValue,bladeGreenValue,bladeBlueValue);
      flashColor = strip.Color(193,255,10);
      break;
    case 3:
      bladeRedValue = 255;
      bladeGreenValue = 0;
      bladeBlueValue = 0;
      bladeColor = strip.Color(bladeRedValue,bladeGreenValue,bladeBlueValue);
      flashColor = strip.Color(0,255,210);
      break;
    case 4:
      //code
      break;
    default:
      bladeRedValue = 0;
      bladeGreenValue = 255;
      bladeBlueValue = 0;
      bladeColor = strip.Color(bladeRedValue,bladeGreenValue,bladeBlueValue);
      flashColor = strip.Color(255,0,0);
      break;
  }
}
void runLightsaber(){
  // if lightsaberEnabled == true and bladeExtended == false
  //   extend blade
  // else lightsaberEnabled == true and bladeExtended == true
  //   run motion fx
  // else lightsaberEnabled == false and bladeExtended == true
  //   detract blade
  // else (lightsaberEnabled is false and bladeExtended = false)
  //   do nothing

  if(lightsaberEnabled == true && bladeExtended == false){
    extendBlade();
    bladeExtended = true;
  }
  else if (lightsaberEnabled == true && bladeExtended == true){
    motionFX();
  }else if(lightsaberEnabled == false && bladeExtended == true){
    retractBlade();
    bladeExtended = false;
   
  }
}

void extendBlade(){
  if (colorMode == 4){
    for (int i=0; i<NUM_PIXELS;i++){
      // if i is even, set pixelColor to green
      // else set pixelColor to red
      if (i %2 == 0){
        strip.setPixelColor(i, 0, 255, 0);
      }else{
        strip.setPixelColor(i, 255, 0, 0);
      }
      
      strip.show();
      delay(50);
    }
  }else{
    for (int i=0; i<NUM_PIXELS;i++){
      strip.setPixelColor(i, bladeColor);
      strip.show();
      delay(50);
    }
  }
}

void retractBlade(){
  for (int i=NUM_PIXELS-1; i>=0;i--){
      uint32_t colorValue = strip.Color(0,0,0);
      strip.setPixelColor(i, colorValue);
      strip.show();
      delay(50);
   }
}
void chasingPattern(){
  for (int i=0; i<NUM_PIXELS; i++){
    strip.setPixelColor(i, 78,230,150); // mint green?
    strip.show();
    delay(20);
  }
  
  delay(4000);
  
  for (int i=0; i<NUM_PIXELS; i++){
    strip.setPixelColor(i, 220,70,130); // purple?
    strip.show();
    delay(20); 
  }

  delay(4000);
}

void fadeInOut(){
  uint32_t colorValue;
  for (int i=0; i<=255;i+=5){
    colorValue = strip.Color(i,0,i);
    strip.fill(colorValue,0,NUM_PIXELS);
    strip.show();
    delay(20);
  }

  for (int i=255; i>=0;i-=5){
    colorValue = strip.Color(i,0,i);
    strip.fill(colorValue,0,NUM_PIXELS);
    strip.show();
    delay(20);
  }
}

void motionFX(){
  uint32_t colorValue;
  float pitch;
  float lightValue;
  float motionValue = abs(CircuitPlayground.motionY()) + abs(CircuitPlayground.motionZ());
  Serial.println(motionValue);
  if (motionValue >= 17){
    for(int i=0; i<4;i++){
      strip.fill(flashColor);
      strip.show();
      // i = 1                    2       3          4
      //     500, 750
      for(int j=0; j<2; j++){
        CircuitPlayground.playTone(500 + 1000*i + 250*j, 5);
        delay(5);
      }
      strip.fill(bladeColor);
      strip.show();
      // i =  1                   2        3         4
      //      1000, 1250
      for(int j=0; j<2; j++){
        CircuitPlayground.playTone(1000 + 1000*i + 250*j, 5);
        delay(5);
      }
     
    }
  }
  else{
    if (colorMode == 4){
      for (int i=0; i<NUM_PIXELS;i++){
        if (i %2 == 0){
          strip.setPixelColor(i, 0, 255, 0);
        }else{
          strip.setPixelColor(i, 255, 0, 0);
        }
      }
       strip.show();
       delay(15);
    }else{
      lightValue = map(motionValue, 2, 15, 50, 255);
      lightValue = constrain(lightValue, 50, 255);
      colorValue = strip.Color(bladeRedValue * lightValue/255, bladeGreenValue * lightValue/255, bladeBlueValue * lightValue/255);
      strip.fill(colorValue, 0, NUM_PIXELS);
      strip.show();
      pitch = map(motionValue, 2, 15, 262, 300);
      CircuitPlayground.playTone(pitch, 15);
      delay(15);
    }
  }
}


  
  
