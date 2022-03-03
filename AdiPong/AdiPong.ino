#include "Arduboy2.h"
#include "ArduboyTones.h"

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

int ballRadius = 3;
int ballX = 0; //arduboy.width()/2;
int ballY = arduboy.height()/2;
float ballSpeedX = 2;
float ballSpeedY = 2;

int p1PaddleY = arduboy.height()/2;
int p2PaddleY = arduboy.height()/2;
int p1PaddleHeight = 12;
int p2PaddleHeight = 8;
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
  if (arduboy.pressed(UP_BUTTON)){
    if (p1PaddleY > p1PaddleHeight/2){
      p1PaddleY -= 4;
    }else{
      p1PaddleY = p1PaddleHeight/2;
    }
    
  }
  if (arduboy.pressed(DOWN_BUTTON)){
    if (p1PaddleY < arduboy.height() - p1PaddleHeight/2){
      p1PaddleY += 4;
    }else{
      p1PaddleY = arduboy.height() - p1PaddleHeight/2;
    }
      
  }
}

void moveAllThings(){
  if (ballY < ballRadius || ballY > arduboy.height() - ballRadius){
    ballSpeedY = -ballSpeedY;
  }
  if (ballX > arduboy.width() - ballRadius){
    ballSpeedX = -ballSpeedX;
  }

  // if ballx and ballY match paddleX(4) and paddleY
  // then reverse the ballX
  if (ballX <= 0){
    ballX = arduboy.width()/2;
    ballSpeedX = -ballSpeedX;
  }
  else if(ballX <= 4 + 3 + ballRadius && 
     ballY >= p1PaddleY - p1PaddleHeight/2 &&
     ballY <= p1PaddleY + p1PaddleHeight/2){
    ballSpeedX = -ballSpeedX;
  }

  ballX = ballX + ballSpeedX;
  ballY = ballY + ballSpeedY;
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
  arduboy.fillRect(4, p1PaddleY - p1PaddleHeight/2,
                   3, p1PaddleHeight, WHITE);
  arduboy.fillRect(arduboy.width()-4, p2PaddleY - p2PaddleHeight/2,
                   3 ,p2PaddleHeight, WHITE);
}
