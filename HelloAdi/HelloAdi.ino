/*
Hello, World! example
June 11, 2015
Copyright (C) 2015 David Martinez
All rights reserved.
This code is the most basic barebones code for writing a program for Arduboy.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
*/

#include "Arduboy2.h"
#include "ArduboyTones.h"

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

int ballRadius = 3;
int ballX = 1;
int ballY = 1;
float ballSpeedX = 4;
float ballSpeedY = 4;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(15);
  //arduboy.digitalWriteRGB(RGB_OFF,RGB_ON,RGB_OFF);
  //arduboy.digitalWriteRGB(BLUE_LED, RGB_ON);
  arduboy.setRGBled(RGB_OFF,RGB_OFF, RGB_OFF);
  arduboy.setRGBled(0, 0, 0);
  sound.tone(440,1000);
}


void loop() {
  if (!(arduboy.nextFrame()))
    return;

  arduboy.clear();
  //arduboy.setCursor(4, 9);
  //arduboy.print(F("Hello, Adi!"));

  handleInputs();
  moveAllThings();
  drawAllThings();
}

// ==== HANDLE INPUTS ====
void handleInputs(){
  if (arduboy.pressed(RIGHT_BUTTON)){
    ballX+=ballSpeedX;
  }
  if (arduboy.pressed(LEFT_BUTTON)){
    ballX-=ballSpeedX;
  }
  if (arduboy.pressed(UP_BUTTON)){
    ballY-=ballSpeedY;
  }
  if (arduboy.pressed(DOWN_BUTTON)){
    ballY+=ballSpeedY;
  }
}

// ==== MOVE ALL THINGS ====
void moveAllThings(){
  // ===== WRAP AROUND FOR RIGHT AND LEFT EDGES =====
  if(ballX < 0){
    //ballSpeedX = ballSpeedX * -1;
    ballX = arduboy.width();
    arduboy.setRGBled(0, 255, 255);
    sound.tone(700,2000);
    //ballRadius += 1;
  }else if(ballX > arduboy.width()){
    ballX = 0;
    arduboy.setRGBled(0,255,0);
    sound.tone(700,2000);
  }else{
    arduboy.setRGBled(0,0,0);
  }

  // ==== STOP ON EDGE FOR TOP AND BOTTOM EDGES =====
  if(ballY < ballRadius){                         // Check top collision
    //ballSpeedY = ballSpeedY * -1;
    ballY = ballRadius;
    arduboy.setRGBled(RED_LED,255);
  }else if (ballY>arduboy.height()- ballRadius){ //if not top, check bottom
    ballY = arduboy.height() - ballRadius;
    arduboy.setRGBled(RED_LED,255);
  }else{
    arduboy.setRGBled(RED_LED,0);                //if none of the above
  }
  
}

// === DRAW ALL THINGS ===
void drawAllThings(){
  arduboy.setCursor(4, 56);
  arduboy.print("X: " + (String)ballX + " | Y: " + (String)ballY);
  arduboy.fillCircle(ballX,ballY,ballRadius,WHITE);
  //ballX += ballSpeedX;
  //ballY += ballSpeedY;
  //ballRadius += 1;
  arduboy.display();
}
