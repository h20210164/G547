Writing IIO driver interfacing of ADXL345 sensor:

Prerequisite for the project

For these files to run we need raspberry pi running on linux kernel. To update and build the kernel use link. First remove the loaded module. Command to remove the module is "sudo rmmod adxl.ko"

List of softwares used for this project's implementation:

Putty
WinSCP
Vnc viewer


Summary

Using IIO subsystems, I2C interfacing of ADXL345 is done. At probe function, memory allocation for IIO device is done and IIO device registration is done. At remove function, IIO device is unregistered and memory is freed. ADXL345 has 3 channels ie x,y,z axis data. The axis data is read from the register addresses 0x32, 0x34, 0x36 using read raw function. Register addresses are available in ADXL345 datasheet. The host used is the Raspberry Pi 3b+ model.




Setup and the schematic.![aa](https://user-images.githubusercontent.com/98520600/166180337-eca865e0-9ad7-4857-9539-25c35223926f.jpg)


Schematic: pi-and-adxl345_bb
![image](https://user-images.githubusercontent.com/98520600/166179884-fafc8444-4a27-451a-9d88-07b97b117643.png)


Setup:

ADXL345
![image](https://user-images.githubusercontent.com/98520600/166179922-ea895bad-56ae-4768-bc76-d3460515daf6.png)


adxl

The ADXL345 is a MEMS producing output proportional to the acceleration that it is subjected to. It is able to measure so along 3 axes (x, y, z). It has 6 8-bit data registers, 2 for each axis, in effect 16 bits allocated to each axis. But not all the bits are used for the data interpretation. Depending upon the configuration of the DATA_FORMAT_REGISTER the number of bits actually associated with the data change. The Enable register also needs to be configured to get the sensor up and running. For further reference, refer the ADXL345 datasheet here.

Connections
ADXL Pin	RaspberryPi Pin
VCC	Pin 1 (3.3 V)
Ground	Pin 39
SCL	Pin 3
SDA	Pin 5
Driver functionality
The driver implemented has ioctl call for

Turning on the device
Reading individual axis data.
Setting the range to be +/-2g, +/-4g, +/-8g and +/- 16g.

Interpreting the data
The data given out by the sensor is in 2's compliment form. If the range is +/- 2g and resolution is 10 bit, then LSB will represent 4g/1024 (mulitpying factor) as an absolute value. Bringing the 2's compliment data back into decimal form and multiplying it by the factor will give us the value in Gs. The multiplying factor will change depending upon the range and the resolution.


Steps
Navigate to the directory conatining the driver and associated Makefile.

1.make

2.sudo modprobe  industrialio

3.sudo insmod adxl.ko

4.cd sys/bus/iio/devices/

5.cd  iio\:device0

6.cat in_accel_x_raw

7.cat in_accel_y_raw

8.cat in_accel_z_raw
