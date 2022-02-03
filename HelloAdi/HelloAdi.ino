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
float ballSpeedX = 5;
float ballSpeedY =3;
void loop() {
  if (!(arduboy.nextFrame()))
    return;

  arduboy.clear();
  arduboy.setCursor(4, 9);
  arduboy.print(F("Hello, Adi!"));
  arduboy.fillCircle(ballX,ballY,ballRadius,WHITE);
  // If the ball's x position hits the right edge of the screen
  if(ballX >= arduboy.width() || ballX <= 0){
    //ballSpeedX = ballSpeedX * -1;
    ballX = 0;
    //ballRadius += 1;
  }
  if(ballY >= arduboy.height() || ballY <= 0){
    ballSpeedY = ballSpeedY * -1;
  }
  arduboy.setCursor(4, 20);
  arduboy.print("X: " + (String)ballSpeedX);
  ballX += ballSpeedX;
  ballY += ballSpeedY;
  ballRadius += 1;
  arduboy.display();
  
}
