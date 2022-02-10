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

Arduboy2 arduboy;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(15);
}
int ballRadius = 3;
int ballX = 1;
int ballY = 1;
float ballSpeedX = 4;
float ballSpeedY = 4;

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
    //ballRadius += 1;
  }else if(ballX > arduboy.width()){
    ballX = 0;
  }

  // ==== STOP ON EDGE FOR TOP AND BOTTOM EDGES =====
  if(ballY < ballRadius){
    //ballSpeedY = ballSpeedY * -1;
    ballY = ballRadius;
  }
  if (ballY>arduboy.height()- ballRadius){
    ballY = arduboy.height() - ballRadius;
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
