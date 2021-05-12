
// (c) 2021 Marco lardelli
// This code is licensed under MIT license (see LICENSE.txt for details)



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


// ldr light sensor pins
int leftLightPin = 14;
int rightLightPin = 15;
int frontLightPin = 16;
int backLightPin = 17;

// ambient light level, to be subtracted, measured at start
int leftLightStart = 0;
int rightLightStart = 0;
int frontLightStart = 0;
int backLightStart = 0;

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
}

void CalibrateSensors() {
    //read ambient light levels
  leftLightStart = 1023-analogRead(leftLightPin);
  rightLightStart = 1023-analogRead(rightLightPin);
  frontLightStart = 1023-analogRead(frontLightPin);
  backLightStart = 1023-analogRead(backLightPin);
}


void loop(){
  stop(100);
  blinkSpeed(20);
  delay(5000);  // wait a bit to disconnect from usb before starting motors
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

  
  drive(155,0,400);   // left wheel forward
  delay(1000);
  drive(-155,0,400);  // left wheel backward
  delay(1000);
  drive(0,155,400);   // right wheel forward
  delay(1000);
  drive(0,-155,400);  // right wheel backward
  delay(1000);
 
 
  for (int iter=0;iter<100000;iter++){

    // show light intensity from sensors on LEDs 1-4 (for debugging purposes)
    analogWrite(9, 255 - ((100 - (analogRead(14) / 10.23)) * 2.55));
    analogWrite(10, 255 - ((100 - (analogRead(15) / 10.23)) * 2.55));
    analogWrite(3, 255 - ((100 - (analogRead(16) / 10.23)) * 2.55));
    analogWrite(11, 255 - ((100 - (analogRead(17) / 10.23)) * 2.55));
    
    // read from light sensors, subtract ambient light
    leftLight = 1023 - analogRead(leftLightPin)-leftLightStart;
    rightLight = 1023 - analogRead(rightLightPin)-rightLightStart;
    frontLight = 1023 - analogRead(frontLightPin)-frontLightStart;
    backLight = 1023 - analogRead(backLightPin)-backLightStart;

    leftv = leftLight / 512.0;
    rightv = rightLight / 512.0;
    frontv = frontLight / 512.0;
    backv = backLight / 512.0;

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

    // feed neural network output to motors
    drive(Output[0]*MotorForceMultiplier/10000, Output[1]*MotorForceMultiplier/10000,50);
  }
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
  float pglu_speed = ((min(abs(speed),155)*sign/2.55 + 100)/2)*2.55;
  analogWrite(pin, pglu_speed);
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
