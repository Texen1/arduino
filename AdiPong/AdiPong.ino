#include "Arduboy2.h"
#include "ArduboyTones.h"

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

int ballRadius = 3;
int ballX = arduboy.width()/2;
int ballY = arduboy.height()/2;
float ballSpeedX = 2;
float ballSpeedY = 0;

int paddleWidth = 3;
int p1PaddleY = arduboy.height()/2;
int p2PaddleY = arduboy.height()/2;
int p1PaddleHeight = 15;
int p2PaddleHeight = 15;
int p1PaddleX = 4;
int p2PaddleX = arduboy.width() - p1PaddleX;
int p2PaddleSpeed = 2;
int p1Score = 0;
int p2Score = 0;
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
  if (ballY < ballRadius){
    ballY = ballRadius;
    ballSpeedY = -ballSpeedY;
  }
  else if (ballY > arduboy.height() - ballRadius){
    ballY = arduboy.height() - ballRadius;
    ballSpeedY = -ballSpeedY;
    
  }
  
  if (ballX > arduboy.width() - ballRadius){
    ballX = arduboy.width()/2;
    ballY = arduboy.height()/2;
    ballSpeedX = -ballSpeedX;
    ballSpeedY = random(-paddleWidth/2*0.75, paddleWidth/2*0.75 + 1);
    p1Score++;
  }
  else if(ballX >= p2PaddleX - paddleWidth/2 - ballRadius && 
     ballY >= p2PaddleY - p2PaddleHeight/2 - ballRadius &&
     ballY <= p2PaddleY + p2PaddleHeight/2 + ballRadius){
    ballX = p2PaddleX - paddleWidth/2 - ballRadius;
    ballSpeedX = -ballSpeedX;
    ballSpeedY = (ballY - p2PaddleY)*0.75;
  }

  // if ballx and ballY match paddleX(4) and paddleY
  // then reverse the ballX
  if (ballX <= 0){
    ballX = arduboy.width()/2;
    ballY = arduboy.height()/2;
    ballSpeedX = -ballSpeedX;
    p2Score++;
  }
  else if(ballX <= p1PaddleX + paddleWidth/2 + ballRadius && 
     ballY >= p1PaddleY - p1PaddleHeight/2 - ballRadius &&
     ballY <= p1PaddleY + p1PaddleHeight/2 + ballRadius){
    ballX = p1PaddleX + paddleWidth/2 + ballRadius;
    ballSpeedX = -ballSpeedX;
    ballSpeedY = (ballY - p1PaddleY)*0.75;
  }

  ballX = ballX + ballSpeedX;
  ballY = ballY + ballSpeedY;
  if(ballY < p2PaddleY - p2PaddleHeight/3){
    p2PaddleY = p2PaddleY - p2PaddleSpeed;
  }else if(ballY > p2PaddleY + p2PaddleHeight/3){
    p2PaddleY+=p2PaddleSpeed;
  }
}

void drawAllThings(){
  drawNet();
  drawBall();
  drawPaddles();
  drawScore();
  
}
void drawBall(){
  arduboy.fillCircle(ballX,ballY,ballRadius,WHITE);
}

void drawNet(){
  for (int netY = 0; netY<arduboy.height(); netY+=8){
      arduboy.drawLine(arduboy.width()/2, netY, arduboy.width()/2, netY+2, WHITE);
  }
}
void drawScore(){
  arduboy.setCursor(arduboy.width()/2 - 24, 0);
  arduboy.print((String)p1Score);
  arduboy.setCursor(arduboy.width()/2 + 20, 0);
  arduboy.print((String)p2Score);
}
void drawPaddles(){
  arduboy.fillRect(p1PaddleX - paddleWidth/2, p1PaddleY - p1PaddleHeight/2,
                   paddleWidth, p1PaddleHeight, WHITE);
  arduboy.fillRect(p2PaddleX - paddleWidth/2, p2PaddleY - p2PaddleHeight/2,
                   paddleWidth ,p2PaddleHeight, WHITE);
}
