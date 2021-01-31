# (c) 2021 Marco lardelli
# This code is licensed under MIT license (see LICENSE.txt for details)


from mindstorms import MSHub, Motor, MotorPair, ColorSensor, DistanceSensor, App
from mindstorms.control import wait_for_seconds, wait_until, Timer
from mindstorms.operator import greater_than, greater_than_or_equal_to, less_than, less_than_or_equal_to, equal_to, not_equal_to
import math


# Create your objects here.
hub = MSHub()


# Write your program here.
hub.speaker.beep()


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

color_left = ColorSensor('D')
color_right = ColorSensor('C')
color_front = ColorSensor('F')
color_back = ColorSensor('B')

motor_left = Motor('E')
motor_right = Motor('A')

def drive(left_motor, right_motor, seconds):

    motor_left.start_at_power(check_range(-left_motor))
    motor_right.start_at_power(check_range(right_motor))

    wait_for_seconds(seconds)


def check_range(speed):
    if speed>100:
        return 100
    if speed<-100:
        return -100
    return speed

while True:
    # read sensors
    left_light = color_left.get_ambient_light()
    right_light = color_right.get_ambient_light()
    front_light = color_front.get_ambient_light()
    back_light = color_back.get_ambient_light()

    leftv = left_light / 100
    rightv = right_light / 100
    frontv = front_light / 100
    backv = back_light / 100

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

    # feed neural network output to motors
    drive(int(left_motor_speed), int(right_motor_speed), 0.05)
