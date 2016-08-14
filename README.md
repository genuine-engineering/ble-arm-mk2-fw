
# ROBOT ARM MK2 Mbed OS Firmware

Mechanical design: http://www.thingiverse.com/thing:1454048

![MK2](http://thingiverse-production-new.s3.amazonaws.com/renders/fd/6a/20/49/04/3ed8d51cd6f4b9dd4e7e3726ac0d3a65_preview_featured.jpg)

## Features

- Connect to NRF51822 BLE Control board
- Control up/down, left/right, forward/backward using DeviceMotion (Accel, Gyro, Magentic)
- react-native app with redux, only support ios for now

## Mobile Application for IOS

Check: https://github.com/genuine-engineering/ble-arm-mk2-app

## Install

Please check official `yotta` project and `mbedos` for install dependencies

```
git clone https://github.com/genuine-engineering/ble-arm-mk2-fw
cd ble-arm-mk2-fw
yt build
make flash
```

## REMARK

- 

THIS PROJECT FOR SELF LEARN REACT-NATIVE, AND ON GOING DEVELOPING
