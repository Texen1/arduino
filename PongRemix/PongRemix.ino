#include "EEPROM.h"
#include "SPI.h"
#include "Arduboy.h"

Arduboy arduboy;
#define BALL_RADIUS 10
#define PADDLE_WIDTH 4

double cursorX = 0.0;
double cursorY = 64.0;
bool startGame = false;
bool rgbEnabled = true;
bool soundEnabled = true;
bool cpuEnabled = true;

double ballX = 10.0;
double ballY = 10.0;
double ballSpeedX = 5.0;
double ballSpeedY = 3.0;

char paddleStartHeight = 10;
char paddle1Height = 10;
char paddle2Height = 10;
char paddle1Y = HEIGHT/2 - paddle1Height/2;
char paddle2Y = HEIGHT/2 - paddle2Height/2;
char paddle1Speed = 2;
char paddle2Speed = 2;
char cpuSpeed = 1;


unsigned int player1Score = 0;
unsigned int player2Score = 0;

byte redValue = 0;
byte greenValue = 0;
byte blueValue = 0;

// Sounds
const byte PROGMEM bing [] = {
  0x90,65, 0,40, 0x80, 0x90,65, 0, 60, 0x80, 0xf0
};
const byte PROGMEM point [] = {
  0x90,83, 0,60, 0x80, 0x90,88, 0,80, 0x80, 0xf0
};
const byte PROGMEM hit [] = { 
  0x90,60, 0,31, 0x80, 0x90,60, 0,31, 0x80, 0x90,60, 0,31, 0x80, 0xf0
};

// Bitmaps
PROGMEM const unsigned char tslogo[] = {
	0xCE,0xCA,0xCE,0x90,0xA7,0xFF,0x07,0x00,0xE0,0x10,0xEE,0xAE,0xEE,0x00,0x38,0xF8,
	0x38,0x00,0x1C,0xFC,0x5C,0x20,0x10,0x18,0x97,0x95,0x97,0x38,0x38,0x38,0x00,0x00,
	0xC1,0xDD,0xDD,0x9C,0xA0,0xC0,0x81,0x82,0xFF,0x01,0x70,0xF0,0x70,0x00,0xE0,0xE3,
	0xE4,0x0A,0xF1,0x10,0x38,0x28,0x38,0x00,0x83,0x7F,0x13,0x08,0x07,0xE7,0xA7,0xE0,
	0x01,0xC1,0xC1,0xDC,0x94,0x9C,0xA0,0xC0,0xB8,0xF9,0x3A,0x07,0xF8,0x00,0xC0,0x20,
	0x90,0x49,0x27,0x10,0x08,0x04,0x06,0x05,0x04,0x04,0x04,0x06,0x05,0x0E,0x0E,0x0E,
	0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0xFE,0x00,0xFF,0x00,0xFF,0x00,
	0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
}; 

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  playSound(point);
  //Serial.begin(9600);
}

void loop() {
  if (!(arduboy.nextFrame()))
    return;

  arduboy.clear();
  
  if (startGame == false){
  	intro();
  }
  else{
  	runGame();
  }
  
  // then we finaly we tell the arduboy to display what we just wrote to the display
  arduboy.display();
}

void intro(){
  arduboy.setCursor(4, cursorY);
  arduboy.drawBitmap(4, cursorY, tslogo, 32,32, WHITE);
  arduboy.setCursor(70,cursorY + 8);
  arduboy.print(F("Adi"));
  arduboy.setCursor(56,cursorY + 18);
  arduboy.print(F("PRESENTS"));
  arduboy.setCursor(5, cursorY + 34);
  arduboy.print(F("======= PONG ======="));
  arduboy.setCursor(0, cursorY + 46);
  arduboy.print(F("A: RGB=On  B: SFX=On"));
  arduboy.setCursor(4, cursorY + 56);
  arduboy.print(F("Press RIGHT to Start"));
  
  if(rgbEnabled==false){
    arduboy.setCursor(42, cursorY + 46);
    arduboy.print(F("Off"));
  }
  
  if(soundEnabled==false){
    arduboy.setCursor(108, cursorY + 46);
    arduboy.print(F("Off"));
  }
  if (cursorY > 0){
  	cursorY = cursorY - 0.5;
  }
  
  if(arduboy.pressed(RIGHT_BUTTON)){
  	startGame = true;
  }
  if(arduboy.pressed(LEFT_BUTTON)){
  	cpuEnabled = false;
  }
  if(arduboy.pressed(A_BUTTON)){
  	rgbEnabled = !rgbEnabled;
  	debounceButtons();
  }
  if(arduboy.pressed(B_BUTTON)){
  	soundEnabled = !soundEnabled;
  	debounceButtons();
  }
}

void runGame(){
  handleInput();
  moveAllTheThings();
  drawAllTheThings();
  blinkAllTheThings();
}

void handleInput(){
  if(arduboy.pressed(UP_BUTTON) && paddle1Y > 0){
  	paddle1Y = paddle1Y - paddle1Speed;
  }	
  if(arduboy.pressed(DOWN_BUTTON) && paddle1Y < HEIGHT - paddle1Height){
  	paddle1Y = paddle1Y + paddle1Speed;
  }
  if (cpuEnabled == true){
    if(arduboy.pressed(A_BUTTON)){
      rgbEnabled = !rgbEnabled;
      debounceButtons();
    }
    if(arduboy.pressed(B_BUTTON)){
      soundEnabled = !soundEnabled;
      debounceButtons();
    }
  }else{
    if(arduboy.pressed(B_BUTTON) && paddle2Y > 0){
      paddle2Y = paddle2Y - paddle2Speed;
    }
    if(arduboy.pressed(A_BUTTON) && paddle2Y < HEIGHT - paddle2Height){
      paddle2Y = paddle2Y + paddle2Speed;
    }
  }
}

void moveAllTheThings(){
  if (cpuEnabled == true){
    computerMovement();
  }
  
  resizePaddles();
  
  ballX = ballX + ballSpeedX;
  ballY = ballY + ballSpeedY;
  
  if(ballX < BALL_RADIUS + PADDLE_WIDTH){ 
    if(ballX > PADDLE_WIDTH/2 && ballY > paddle1Y && ballY < paddle1Y + paddle1Height){
      playSound(point);
  	  ballSpeedX = -ballSpeedX;
  	  double deltaY = ballY - (paddle1Y + paddle1Height/2);
  	  ballSpeedY = deltaY*0.4;
  	  greenValue = 80;
      blueValue = 80;
      redValue = 80;
    }
  	else if(ballX <= 0){
      playSound(bing);
      player2Score++;
      ballReset();
    }
  }
  
  if(ballX > WIDTH - BALL_RADIUS - PADDLE_WIDTH){
    if(ballX < WIDTH - PADDLE_WIDTH/2 && ballY > paddle2Y && ballY < paddle2Y + paddle2Height){
      playSound(point);
      ballSpeedX = -ballSpeedX;
      double deltaY = ballY - (paddle2Y + paddle2Height/2);
      ballSpeedY = deltaY*0.4;
      greenValue = 80;
    }
    else if(ballX >= WIDTH){
      playSound(bing);	
      player1Score++;
      ballReset();
    }
  }
  
  if(ballY < BALL_RADIUS){
    ballSpeedY = -ballSpeedY;
    //greenValue = 80;
    redValue = 50;
    blueValue = 90;
    playSound(hit);
  }
  else if(ballY > HEIGHT - BALL_RADIUS){
    ballSpeedY = -ballSpeedY;
    greenValue = 80;
    redValue = 80;
    //blueValue = 80;
    playSound(hit);
  }
}

void resizePaddles(){
  if ((int(player2Score)-int(player1Score))%5 == 0){
    paddle1Height = paddleStartHeight + 2*(int(player2Score)- int(player1Score))/5;
    paddle2Height = paddleStartHeight + 2*(int(player1Score) - int(player2Score))/5;
  }
  //Serial.println("Paddle1: " + String(int(paddle1Height)) + " Paddle2: " + String(int(paddle2Height)));
}

void drawAllTheThings(){
  drawNet();
  drawScore();	
  arduboy.fillCircle(ballX,ballY,BALL_RADIUS,WHITE);
  arduboy.fillRoundRect(0,paddle1Y+2,PADDLE_WIDTH,paddle1Height-4,2,WHITE);
  arduboy.fillRoundRect(WIDTH-PADDLE_WIDTH,paddle2Y+2,PADDLE_WIDTH,paddle2Height-4,2,WHITE);
}

void blinkAllTheThings(){
  if (rgbEnabled == true){
    arduboy.setRGBled(redValue,greenValue,blueValue);
    if (redValue > 0){
      redValue -= 5;
    }
    if (greenValue > 0){
      greenValue -= 5;
    }
    if (blueValue > 0){
      blueValue -= 5;
    }
  }
  else{
    arduboy.setRGBled(0,0,0);
  }
}

void drawScore(){
  arduboy.setCursor(WIDTH/2 - 25, 5);
  arduboy.print(player1Score); 
  arduboy.setCursor(WIDTH/2 + 20, 5);
  arduboy.print(player2Score);
}

void drawNet(){
  for(char i=0;i<HEIGHT;i+=10){
    arduboy.fillRect(WIDTH/2-1,i,2,4,WHITE);
  }
}

void ballReset(){
  //greenValue = 80;
  redValue = 80;
  blinkAllTheThings();
  ballSpeedX = -ballSpeedX;
  ballSpeedY = 0;
  ballX = WIDTH/2;
  ballY = HEIGHT/2;
  delay(500);
}

void computerMovement(){
  char paddle2YCenter = paddle2Y + paddle2Height/2;
  if(paddle2YCenter < ballY - 6){
    paddle2Y += cpuSpeed;
  }
  else if(paddle2YCenter > ballY + 6){
    paddle2Y -= cpuSpeed;
  }
}

void playSound(const byte *score){
  if(arduboy.tunes.playing()){
    arduboy.tunes.stopScore();
  }
  if (soundEnabled == true){
    arduboy.tunes.playScore(score);
  }
}

void debounceButtons() { // prevent "noisy" buttons from appearing as multiple presses
  delay(100);
  while (arduboy.buttonsState()) { }  // Wait for all keys released
  delay(100);
}
