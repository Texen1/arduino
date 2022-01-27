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

int ballX = 0;
void loop() {
  if (!(arduboy.nextFrame()))
    return;

  arduboy.clear();
  arduboy.setCursor(4, 9);
  arduboy.print(F("Hello, Adi!"));
  arduboy.fillCircle(ballX,32,10,WHITE);
  ballX += 1;
  arduboy.display();
}
