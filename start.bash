#!/bin/bash
sudo bash -c 'echo "/tmp/core-%e-%p-%t" >  /proc/sys/kernel/core_pattern' && sudo bash -c "ulimit -c unlimited && /home/nv/jetson_car_joystick/build/nano-joystick"
