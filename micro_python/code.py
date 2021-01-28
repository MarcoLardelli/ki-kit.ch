
# (c) 2021 Marco lardelli
# This code is licensed under MIT license (see LICENSE.txt for details)


import board
import digitalio
import time
import pulseio
import busio
import adafruit_ssd1306
from analogio import AnalogIn

# COMPUTER GENERATED CODE BELOW THIS LINE - DO NOT EDIT MANUALLY!
# ---------------------------------------------------------------

# HYPERPARAMETERS USED FOR TRAINING:
# Generation: 79 of 200
# No of experiments per generation: 448
# Spot light displacement: 3.479455
# Angular velocity punishment: 10000
# Runtime (episode duration in seconds): 4
# Initial weight variance: 4
# Final weight variance: 0.25
# Motor force multiplier: 10
# Number of Hidden neurons: 12
# Output activation function: NONE
# Include t minus one second: False

# INFO
# Number of generations survived: 11

InputNodes = 4
HiddenNodes = 12
OutputNodes = 2
MotorForceMultiplier = 10
InputScale = 10000

HiddenWeights = [
  [38.21041, -19.60326, -19.34315, -30.43454, -11.25313, -7.807495,
   -18.68385, 0.02767038, -16.48638, 18.0504, -21.92417, -13.54609],
  [14.21366, -13.46543, 15.89999, -24.21149, -15.46361, 0.1029675,
   -32.01714, -4.0213, 1.348504, 10.9667, -14.09514, -24.37091],
  [-1.129683, -19.91963, 13.1478, 17.04446, 9.783686, 5.926507,
   12.98946, -13.01615, 0.4301937, 24.6978, 17.51382, -26.93697],
  [16.97503, -27.18933, 22.51009, -9.452932, 55.90073, -32.39577,
   35.47231, -9.687993, -12.86657, -17.01292, -15.86054, 20.37926],
  [27.72655, -15.24753, -15.3898, -2.753998, 18.72049, -9.177473,
   25.03577, -2.756675, 4.04003, -17.62782, -38.66041, 3.362186]
]

OutputWeights = [
  [7.000915, 14.9469],
  [-32.24245, 38.10791],
  [-26.36042, -19.04521],
  [-8.630703, 2.323947],
  [-36.42758, -28.91781],
  [-27.82501, -19.62623],
  [-20.60663, -1.936434],
  [-22.19947, -14.54366],
  [26.88729, -20.87098],
  [47.68628, 32.83581],
  [-48.05303, -5.74169],
  [20.55724, -9.699068],
  [-26.58092, 20.82998]
]

Input = [0, 0, 0, 0]
Hidden = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
Output = [0, 0]

# END OF COMPUTER GENERATED CODE
# ---------------------------------------------------------------

motor_test_sequence = False

# motor 1
M1direction = digitalio.DigitalInOut(board.D13)
M1direction.direction = digitalio.Direction.OUTPUT
M1speed = pulseio.PWMOut(board.D11, frequency=1000, duty_cycle=1)
# M1speed = digitalio.DigitalInOut(board.D11)
# M1speed.direction = digitalio.Direction.OUTPUT

# motor 2
M2direction = digitalio.DigitalInOut(board.D10)
M2direction.direction = digitalio.Direction.OUTPUT
M2speed = pulseio.PWMOut(board.D9, frequency=1000, duty_cycle=1)

# motor test

# initialize display
i2c = busio.I2C(board.SCL, board.SDA)
oled = adafruit_ssd1306.SSD1306_I2C(128, 64, i2c, addr=0x3c)

oled.fill(0)
oled.text('Starting', 0, 0)
oled.text('Calib. in 2s!', 0, 10)
oled.show()

time.sleep(2)

oled.fill(0)
oled.text('Calib. now!', 0, 0)
oled.show()
time.sleep(1)

left_sensor = AnalogIn(board.A1)
right_sensor = AnalogIn(board.A2)
front_sensor = AnalogIn(board.A3)
back_sensor = AnalogIn(board.A4)

left_sensor_start = left_sensor.value
right_sensor_start = right_sensor.value
front_sensor_start = front_sensor.value
back_sensor_start = back_sensor.value

oled.fill(0)
oled.text('L: '+str(int(left_sensor_start/100)), 0, 0)
oled.text('R: '+str(int(right_sensor_start/100)), 0, 10)
oled.text('F: '+str(int(front_sensor_start/100)), 0, 20)
oled.text('B: '+str(int(back_sensor_start/100)), 0, 30)
oled.show()

time.sleep(2)


def stop():
    M1direction.value = False
    M1speed.duty_cycle = 0
    M2direction.value = False
    M2speed.duty_cycle = 0

def drive(left_motor, right_motor, seconds):
    left_speed = min(abs(left_motor/4), 65)
    right_speed = min(abs(right_motor/4), 65)

    # oled.fill(0)
    # oled.text('M-L: '+str(int(left_motor)), 0, 0)
    # oled.text('M-R: '+str(int(right_motor)), 0, 10)
    # oled.show()

    if left_motor > 0:
        M1direction.value = True
        M1speed.duty_cycle = int(65535 - left_speed * 1000)
    else:
        M1direction.value = False
        M1speed.duty_cycle = int(left_speed * 1000)

    if right_motor > 0:
        M2direction.value = True
        M2speed.duty_cycle = int(65535 - right_speed * 1000)
    else:
        M2direction.value = False
        M2speed.duty_cycle = int(right_speed * 1000)

    time.sleep(seconds)

# motor test sequence
stop()
if motor_test_sequence:
    drive(155, 0, 4)  # left wheel forward
    time.sleep(1)
    drive(-155, 0, 4)  # left wheel backward
    time.sleep(1)
    drive(0, 155, 4)   # right wheel forward
    time.sleep(1)
    drive(0, -155, 4)  # right wheel backward
    time.sleep(1)

iter = 0
while True:
    # read sensors
    left_light = left_sensor.value-left_sensor_start
    right_light = right_sensor.value-right_sensor_start
    front_light = front_sensor.value-front_sensor_start
    back_light = back_sensor.value-back_sensor_start

    if iter % 10 == 0:
        oled.fill(0)
        oled.text('Left  :'+str(int(left_light/100)), 0, 0)
        oled.text('Right :'+str(int(right_light/100)), 0, 10)
        oled.text('Front :'+str(int(front_light/100)), 0, 20)
        oled.text('Back  :'+str(int(back_light/100)), 0, 30)

    leftv = left_light / 32500
    rightv = right_light / 32500
    frontv = front_light / 32500
    backv = back_light / 32500

    Input[0] = frontv*InputScale
    Input[1] = leftv*InputScale
    Input[2] = rightv*InputScale
    Input[3] = backv*InputScale

    # compute hidden layer activations

    for i in range(HiddenNodes):
        Accum = HiddenWeights[InputNodes][i]
        for j in range(InputNodes):
            Accum += Input[j] * HiddenWeights[j][i]
        # leaky ReLU
        if Accum < 0:
            Hidden[i] = 0.1*Accum
        else:
            Hidden[i] = Accum
            # Hidden[i] = 1.0/(1.0 + exp(-Accum))

    # Compute output layer activations

    for i in range(OutputNodes):
        Accum = OutputWeights[HiddenNodes][i]
        for j in range(HiddenNodes):
            Accum += Hidden[j] * OutputWeights[j][i]

        # Output[i] = 1.0/(1.0 + exp(-Accum)) ;
        # no activation function
        Output[i] = Accum

    left_motor_speed = Output[0]*MotorForceMultiplier/50000
    right_motor_speed = Output[1]*MotorForceMultiplier/50000

    if (iter % 10) == 0:
        oled.text("M-L:"+str(left_motor_speed), 0, 40)
        oled.text("M-R:"+str(right_motor_speed), 0, 50)
        oled.show()

    # feed neural network output to motors
    drive(left_motor_speed, right_motor_speed, 0.05)

    iter += 1
