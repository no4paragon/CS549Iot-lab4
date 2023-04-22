
<h1>Lab 4: Bluetooth Step Counter</h1>
Your task is to build a step counter using an ESP32 board and send the number of steps to a cell phone using the built-in BLE module, whenever it changes. The lab instruction includes two phases:<br>
a) connecting the accelerometer board to the ESP32 board<br>
b) sending BLE packets to a cell phone<br>

# Deliverables:

## Part A: Turn on and off an LED using the cell phone.

Develop a client-server communication between ESP32 and a cellphone. 

You can either use the onboard LED or hook one up on the breadboard. 

Develop two commands: one to switch LED on, and another to switch LED off.

<h2>[lab4 part A video](https://youtu.be/dQdGrQpl1_M)</h2>

<img src="https://github.com/no4paragon/CS549Iot-lab4/blob/main/partA1.jpg" width="40%" height="40%">
<img src="https://github.com/no4paragon/CS549Iot-lab4/blob/main/partA2.jpg" width="40%" height="40%">

## Part B- Develop a very simple Bluetooth step counter

Using the LSM6DS3 sensor and ESP32 based TTGO, count number of steps by mimicking the step motion and sent it by Bluetooth to cell phone. Please follow the below steps:

- You need to calibrate the sensor (Remember how you calibrate a sensor and mapped it).
- In the loop, continuously read the data (for example each 20 ms). You can just read the data from one of the axes, or read both x and y and calculate their root mean square.
- To count the number of steps, you need to set a threshold. Note that for each step, there might be more than one value over the threshold. Count the number of steps using this threshold. Print the number of steps in a terminal.
- Send the number of taken steps to the cell phone using the built-in BLE.

⚠️ Please do not use use any *pedometer* library to make the step counter. You need to implement this on your own.


<h2>[lab4 part B video1](https://youtu.be/nVnQIvxz-M4)</h2>
<h2>[lab4 part B video2](https://youtu.be/fV3TQLiQD0E)</h2>

<img src="https://github.com/no4paragon/CS549Iot-lab4/blob/main/partB.jpg" width="40%" height="40%">
