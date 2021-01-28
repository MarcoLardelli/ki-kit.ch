
// (c) 2018-2021 Marco lardelli
// This code is licensed under MIT license (see LICENSE.txt for details)


int motorAPin_A = 8; //Arduino digital 8 is connected to HG7881's A-1A terminal
int motorAPin_B = 9; //Arduino digital 9 is connected to HG7881's A-1B terminal

int motorBPin_A = 5; //Arduino digital 5 is connected to HG7881's A-1A terminal
int motorBPin_B = 6; //Arduino digital 6 is connected to HG7881's A-1B terminal



void setup(){
  
  pinMode(motorAPin_A, OUTPUT); //direction
  pinMode(motorAPin_B, OUTPUT); //speed
  pinMode(motorBPin_A, OUTPUT); //direction
  pinMode(motorBPin_B, OUTPUT); //speed

  pinMode(LED_BUILTIN, OUTPUT);

}


void loop(){
  stop(100);
  blinkSpeed(20);
  delay(10000);  // wait a bit to disconnect from usb before starting motors
  blinkSpeed(200);
  
  // this is a small test to check if the motor wiring is correct
  drive(155,0,400);   // left wheel forward
  delay(1000);
  drive(-155,0,400);  // left wheel backward
  delay(1000);
  drive(0,155,400);   // right wheel forward
  delay(1000);
  drive(0,-155,400);  // right wheel backward
  delay(1000);
}

void drive(float leftMotor, float rightMotor, int time) {
   
   int leftSpeed = min(abs(leftMotor),155);
   int rightSpeed = min(abs(rightMotor),155);

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
