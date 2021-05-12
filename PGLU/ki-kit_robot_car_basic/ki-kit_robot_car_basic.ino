




void setup(){

  pinMode(LED_BUILTIN, OUTPUT);

}


void loop(){
  stop(100);
  blinkSpeed(20);
  delay(5000);  // wait a bit to disconnect from usb before starting motors
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
   pglu_motor(6, leftMotor);
   pglu_motor(5, rightMotor);
  delay(time);
}

void stop(int delayAfterStop){
  // set both direction and pwm to low
  pglu_motor(5,0);
  pglu_motor(6,0);
  delay(delayAfterStop);
}

// speed: -255..255
void pglu_motor(int pin, float speed){
  float sign;
  if (speed>0){
    sign = 1;
  } else {
    sign = -1;
  }
  float pglu_speed = ((min(abs(speed),255)*sign/2.55 + 100)/2)*2.55;
  analogWrite(pin, pglu_speed);
}

void blink(){
  for(int i=0; i<=5; i++){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for 1/10 second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100); 
  }
}

void blinkSpeed(int speed){
  for(int i=0; i<=5; i++){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(255-speed);                  // wait
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(255-speed); 
  }
}
