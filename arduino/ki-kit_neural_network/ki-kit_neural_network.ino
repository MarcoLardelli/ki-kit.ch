
// (c) 2021 Marco lardelli
// This code is licensed under MIT license (see LICENSE.txt for details)
// Neural network loops from http://robotics.hobbizine.com/arduinoann.html (Author: Ralph Heymsfeld)

// ------------ computer generated code below this line, DO NOT MODIFY ------------

// META PARAMETERS USED FOR TRAINING
// Generation: 143 of 200
// No of experiments per generation: 300
// Spot light displacement: 3.5
// Angular velocity punishment: 10000
// Runtime (episode duration in seconds): 4
// Initial weight max: 4
// Final weight max: 0.25
// Motor force multiplier: 10
// Number of Hidden nodes: 10
// Output activation function: NONE
// Include t minus one second: False

// INFO
// Number of generations survived: 14

const int InputNodes = 4;
const int HiddenNodes = 10;
const int OutputNodes = 2;
const float MotorForceMultiplier = 10;
const float InputScale = 10000;

float HiddenWeights[InputNodes+1][HiddenNodes] = {
  {-10.5071 ,-28.6881 ,49.60723 ,-23.88357 ,-39.64715 ,-18.24572 ,-27.01153 ,-25.99557 ,15.14673 ,13.87427},
  {1.652861 ,-13.10295 ,6.646737 ,7.469599 ,33.07862 ,-2.438618 ,-10.64128 ,18.80143 ,31.26506 ,-14.81889},
  {21.20042 ,-13.02495 ,-11.88434 ,-12.08691 ,-23.28734 ,-34.93689 ,25.73363 ,-8.896099 ,-37.03425 ,-17.514},
  {-19.2657 ,-10.00093 ,-13.0666 ,23.542 ,-0.7619491 ,-26.32526 ,50.7839 ,16.5293 ,4.468988 ,-20.49962},
  {23.45566 ,19.99741 ,0.2138262 ,6.149847 ,6.500648 ,9.950813 ,-20.71023 ,31.42357 ,15.78693 ,27.31748}
};

float OutputWeights[HiddenNodes+1][OutputNodes] = {
  {6.557656 ,1.594061},
  {7.629223 ,-0.2973841},
  {32.28601 ,27.33524},
  {0.560276 ,-0.8649625},
  {-48.71197 ,-2.280177},
  {24.57121 ,-10.00805},
  {-8.880691 ,-13.53585},
  {-30.56511 ,-25.8589},
  {-5.349677 ,-11.72226},
  {19.98512 ,-1.323637},
  {4.43928 ,25.86219}
};

float Input[InputNodes] = {0,0,0,0};
float Hidden[HiddenNodes];
float Output[OutputNodes] = {0,0};

// ------------- end of computer generated code ----------


int motorAPin_A = 8; //Arduino digital 8 is connected to H-Bridge's A-1A terminal
int motorAPin_B = 9; //Arduino digital 9 is connected to H-Bridge's A-1B terminal

int motorBPin_A = 5; //Arduino digital 5 is connected to H-Bridge's B-1A terminal
int motorBPin_B = 6; //Arduino digital 6 is connected to H-Bridge's B-1B terminal


// ldr light sensor pins
int leftLightPin = A0;
int rightLightPin = A1;
int frontLightPin = A2;
int backLightPin = A3;

// ambient light level, to be subtracted, measured at start
int leftLightStart = 0;
int rightLightStart = 0;
int frontLightStart = 0;
int backLightStart = 0;


// using this lightweight SSD1306 text only library: https://github.com/greiman/SSD1306Ascii

#include <Wire.h>

#include "SSD1306Ascii.h"

#include "SSD1306AsciiWire.h"

#define I2C_ADDRESS 0x3C

#define RST_PIN -1

SSD1306AsciiWire oled;

void setup(){

  Serial.begin(9600);
  
  pinMode(motorAPin_A, OUTPUT); //direction
  pinMode(motorAPin_B, OUTPUT); //speed
  pinMode(motorBPin_A, OUTPUT); //direction
  pinMode(motorBPin_B, OUTPUT); //speed

  pinMode(LED_BUILTIN, OUTPUT);

 

  Wire.begin();
  Wire.setClock(400000L);

  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);
  oled.clear();
  oled.print("Starting soon....");

}

void CalibrateSensors() {
  oled.clear();
  oled.println("Calibrating sensors....");
  //read ambient light levels
  leftLightStart = analogRead(leftLightPin);
  rightLightStart = analogRead(rightLightPin);
  frontLightStart = analogRead(frontLightPin);
  backLightStart = analogRead(backLightPin);
  oled.print("L: ");oled.println(leftLightStart);
  oled.print("R: ");oled.println(rightLightStart);
  oled.print("F: ");oled.println(frontLightStart);
  oled.print("B: ");oled.println(backLightStart);

  delay(1000);  // give user time to read raw values
}


void loop(){
  stop(100);
  blinkSpeed(20);
  delay(10000);  // wait a bit to disconnect from usb before starting motors
  blinkSpeed(200);
  
  CalibrateSensors();

  int leftLight=0;
  int rightLight=0;
  int frontLight=0;
  int backLight=0;

  float leftv =0;
  float rightv =0;
  float frontv =0;
  float backv =0;

  float Accum;
  int i, j;

  /*
  // this is a small test to check if the motor wiring is correct
  drive(155,0,400);   // left wheel forward
  delay(1000);
  drive(-155,0,400);  // left wheel backward
  delay(1000);
  drive(0,155,400);   // right wheel forward
  delay(1000);
  drive(0,-155,400);  // left wheel backward
  delay(1000);
  */
 
  for (int iter=0;iter<100000;iter++){
    // read from light sensors, subtract ambient light
    leftLight = analogRead(leftLightPin)-leftLightStart;
    rightLight = analogRead(rightLightPin)-rightLightStart;
    frontLight = analogRead(frontLightPin)-frontLightStart;
    backLight = analogRead(backLightPin)-backLightStart;

    leftv = leftLight / 512.0;
    rightv = rightLight / 512.0;
    frontv = frontLight / 512.0;
    backv = backLight / 512.0;

    if ((iter % 10) == 0) {
      oled.clear();
    
      oled.print("       F:");oled.println(frontLight);
      oled.println();
      oled.print("L:");oled.print(leftLight);oled.print("       R:");oled.println(rightLight);
      oled.println();
      oled.print("       B:");oled.println(backLight);
    }

    //Serial.println("start");
    //Serial.println(leftLight);
    //Serial.println(rightLight);
    //Serial.println(frontLight);
    //Serial.println(backLight);
    //Serial.println("end");
    //delay(1000);

    // feed to input layer of neural network
    Input[0] = frontv*InputScale;
    Input[1] = leftv*InputScale;
    Input[2] = rightv*InputScale;
    Input[3] = backv*InputScale;
    
    // Compute hidden layer activations
    
    for( i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[InputNodes][i] ;
      for( j = 0 ; j < InputNodes ; j++ ) {
        Accum += Input[j] * HiddenWeights[j][i] ;
      }

      // leaky ReLU
      if (Accum<0) { 
        Hidden[i] = 0.1*Accum;
      } else {
        Hidden[i] = Accum;
      }
     
      //Hidden[i] = 1.0/(1.0 + exp(-Accum)) ;
    }
    
    // Compute output layer activations
    
    for( i = 0 ; i < OutputNodes ; i++ ) {    
      Accum = OutputWeights[HiddenNodes][i] ;
      for( j = 0 ; j < HiddenNodes ; j++ ) {
        Accum += Hidden[j] * OutputWeights[j][i] ;
      }
      //Output[i] = 1.0/(1.0 + exp(-Accum)) ;
      // no activation function
      Output[i] = Accum ; 
    }

    if ((iter % 10) == 0) {
      oled.print("Motor-L: ");oled.println(Output[0]);
      oled.print("Motor-R: ");oled.print(Output[1]);
    }

    // feed neural network output to motors
    drive(Output[0]*MotorForceMultiplier/10000, Output[1]*MotorForceMultiplier/10000,50);
  }
}

void drive(float leftMotor, float rightMotor, int time) {
   //Serial.println("start");
   
   //Serial.println(leftMotor);
   //Serial.println(rightMotor);
   
   int leftSpeed = min(abs(leftMotor),155);
   int rightSpeed = min(abs(rightMotor),155);

   //Serial.println("end");

   if (leftMotor>0) {
     analogWrite(motorBPin_A, 255);
     analogWrite(motorBPin_B, invertOurValue( leftSpeed ) );
   } else {
    analogWrite(motorBPin_A, LOW);
     analogWrite(motorBPin_B, leftSpeed);
   }

   if (rightMotor>0) {
     analogWrite(motorAPin_A, 255);
     analogWrite(motorAPin_B, invertOurValue( rightSpeed ) );
   } else {
    analogWrite(motorAPin_A, LOW);
     analogWrite(motorAPin_B, rightSpeed);
   }
  delay(time);
}

void stop(int delayAfterStop){
  // set both direction and pwm to low
  analogWrite(motorAPin_A, LOW);
  analogWrite(motorAPin_B, LOW);
  analogWrite(motorBPin_A, LOW);
  analogWrite(motorBPin_B, LOW);
  delay(delayAfterStop);
}

void blink(){
  for(int i=0; i<=5; i++){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100); 
  }
}

void blinkSpeed(int speed){
  for(int i=0; i<=5; i++){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(255-speed);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(255-speed); 
  }
}

int invertOurValue(int input) {
  return 255 - input;
}
