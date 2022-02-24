#include "Arduboy2.h"
#include "ArduboyTones.h"

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

int ballRadius = 3;
int ballX = arduboy.width()/2;
int ballY = arduboy.height()/2;
float ballSpeedX = 4;
float ballSpeedY = 4;

int p1PaddleY = arduboy.height()/2;
int p2PaddleY = arduboy.height()/2;

void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(15);
  //arduboy.setRGBled(RGB_OFF,RGB_OFF,RGB_OFF);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!(arduboy.nextFrame())) 
    return;

  arduboy.clear();
  handleInputs();
  moveAllThings();
  drawAllThings();

  arduboy.display();
}

void handleInputs(){
  //Todo
}

void moveAllThings(){
  //Todo 
}

void drawAllThings(){
  drawNet();
  drawBall();
  drawPaddles();
  
}
void drawBall(){
  arduboy.fillCircle(ballX,ballY,ballRadius,WHITE);
}

void drawNet(){
  for (int netY = 0; netY<arduboy.height(); netY+=8){
      arduboy.drawLine(arduboy.width()/2, netY, arduboy.width()/2, netY+2, WHITE);
  }
}

void drawPaddles(){
  arduboy.fillRect(4, 10, 3, 8, WHITE);
}
