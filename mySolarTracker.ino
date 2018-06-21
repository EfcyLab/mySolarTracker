/*
  Solar Tracker - Dual Axis

 The circuit:
 * Arduino
 * Resistors
 * LDR
 * Solar panel
 * <composants>

 This code is in the public domain.

 Horizontal servo is Bottom servo (PAN)
 Vertical servo is Top servo (TILT)

*/

// #define TRON // Trace activation

// Watch out for connections
#define PINPAN              9
#define PINTILT             10
#define PINLDRTOPLEFT       2 // 2 TL
#define PINLDRTOPRIGHT      3 // 3 TR
#define PINLDRBOTTOMLEFT    0 // 0 BL
#define PINLDRBOTTOMRIGHT   1 // 1 BR

int valueLDRTopLeft       = 0;
int valueLDRTopRight      = 0;
int valueLDRBottomLeft    = 0;
int valueLDRBottomRight   = 0;

int averageValueTop       = 0;
int averageValueBottom    = 0;
int averageValueLeft      = 0;
int averageValueRight     = 0;

int diffAveragePan        = 0;
int diffAverageTilt       = 0;

int stepMovePan           = 5;
int stepMoveTilt          = 5;

//int pause                 = 1000;
int speedMove             = 200;
int tolerance             = 50;

// Include Servo library 
#include <Servo.h>

// Servo definitions
Servo PAN;  // horizontal (left-right)/bottom
Servo TILT; // vertical (up-down)/top

// Standard position servo
int startPositionPan      = 90; //90
int startPositionTilt     = 90; //45
int currentPositionPan    = startPositionPan;
int currentPositionTilt   = startPositionTilt;

// Limit position servo
int limitPositionHighPan  = 175;
int limitPositionLowPan   = 5;
int limitPositionHighTilt = 150;
int limitPositionLowTilt  = 90;

void setup(){
  // Servo attachment
  PAN.attach(PINPAN); 
  TILT.attach(PINTILT);

  // Move servo to start position
  PAN.write(startPositionPan);
  TILT.write(startPositionTilt);

//  delay(pause);
}

void loop(){
  // Read each LDR value
  valueLDRTopLeft     = analogRead(PINLDRTOPLEFT);
  valueLDRTopRight    = analogRead(PINLDRTOPRIGHT);
  valueLDRBottomLeft  = analogRead(PINLDRBOTTOMLEFT);
  valueLDRBottomRight = analogRead(PINLDRBOTTOMRIGHT);
   
  // Calculate average LDR value
  averageValueTop     = (valueLDRTopLeft    + valueLDRTopRight)    / 2;
  averageValueBottom  = (valueLDRBottomLeft + valueLDRBottomRight) / 2;
  averageValueLeft    = (valueLDRTopLeft    + valueLDRBottomLeft)  / 2;
  averageValueRight   = (valueLDRTopRight   + valueLDRBottomRight) / 2;

  // Calculate average difference
  diffAveragePan      = averageValueLeft    - averageValueRight;
  diffAverageTilt     = averageValueTop     - averageValueBottom;

  currentPositionPan  = movePosition(tolerance, 
                                     diffAveragePan, 
                                     averageValueLeft, 
                                     averageValueRight, 
                                     limitPositionHighPan,
                                     limitPositionLowPan,
                                     currentPositionPan,
                                     stepMovePan);
                                     
  currentPositionTilt = movePosition(tolerance, 
                                     diffAverageTilt, 
                                     averageValueTop, 
                                     averageValueBottom, 
                                     limitPositionHighTilt,
                                     limitPositionLowTilt,
                                     currentPositionTilt,
                                     stepMoveTilt);

  PAN.write(currentPositionPan);                                     
  TILT.write(currentPositionTilt); 
  
  delay(speedMove);
}

int movePosition(int tol,
                 int diffAvg,
                 int avgValPosA,
                 int avgValPosB,
                 int limitPosA,
                 int limitPosB,
                 int currentPos,
                 int stepMove){                  
  if (-1*tol > diffAvg || diffAvg > tol){
    if (avgValPosA < avgValPosB){
      currentPos = currentPos + stepMove;
      if (currentPos >= limitPosA){
        currentPos = limitPosA;
      }
    }
    else { // (avgValPosA > avgValPosB)
      currentPos = currentPos - stepMove;
      if (currentPos < limitPosB){
        currentPos = limitPosB;
      }
    }
  }
  return currentPos;
}

