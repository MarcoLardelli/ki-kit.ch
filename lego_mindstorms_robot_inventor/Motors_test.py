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


drive(100,0,1)
drive(-100,0,1)
drive(0,100,1)
drive(0,-100,1)
drive(0,0,1)
