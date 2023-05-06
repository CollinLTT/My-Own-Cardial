# My Own Cardial
A project made for the class CS-596 at San Diego State University in the Spring of 2023.
The iOS app was built for iOS 16.4.1

My Own Cardial is a project that uses an IoT device for monitoring patients which stores the data in a Firebase Realtime Database and has a website and iOS app for access of the data.

### Table of Contents
- [How it Works](#how-it-works)
- [Capabilities](#capabilities)
- [Video Demo](#video-demo)
- [iOS App Information](#ios-app-information)
- [Improvements](#improvements)

## How it Works:

The project device is composed of a LILYGO ESP32 T-Display with a SEN-11574 Pulse Sensor, DHT20 temperature and humidity sensor, two LEDs, a button, and a few jumper wires. It is powered by a USB-C cable connected to a portable battery. Once powered the device connects to Wi-Fi and turns the green LED on. Once the green LED is on, pressing the button will have the device begin to record data from the sensors and blink the red LED for each heartbeat while simultaneously submitting the data to the Firebase Realtime Database. Then a website hosted by Firebase Hosting begins processing the data from the database in real time on the website and formats it appropriately. There is also an iOS app that has one function which is to display the website for mobile screens. Then the person who wants to view the formatted data can log in to the website or iOS app with the proper username and password to see their data.

## Capabilities:

ESP32 IoT Device automatically connects to Wi-Fi then listens for a button to switch between idle state and recording state and includes visual cues in the form of green and red LEDs. 

Using the SEN Pulse Sensor and DHT sensor the device records the heartbeat strength, BPM, temperature, humidity, and timestamp and transmits the data over Wi-Fi to the Firebase Realtime Database. 

The Firebase database correctly and uniquely stores the BPM, heartbeat strength, temperature, humidity, and the timestamp under the UUID associated with the device.

The website hosted by Firebase Hosting displays the BPM, temperature, humidity, and graphs the heart pulse strength from the Realtime Database. Also provides functionality to view historical data and delete the user's data from the database.

The iOS app opens a full screen WebView of the MyOwnCardial website with proper formatting for mobile devices.

## Video Demo:

A video demonstrating the device in action and a walk through of the database and website can be seen here: [Website & Device Demo](https://youtu.be/r_-9DFCSCjI). It first shows the device connecting to Wi-Fi, then a look at the database and website, a look at historical data on the website, then a demonstration of data being recorded in real time.

A video demonstrating the iOS app's function and format can be seen here: [iOS App Demo](https://youtube.com/shorts/5KBZSiFO9Bs?feature=share). It first shows what the app looks like when there is no data in the database then it shows a live intake of data from the device.

## iOS App Information:

#### Requirements for running iOS App:
- Must have an Apple Computer or device with the ability to run XCode.
- Must have an AppleID or be part of the Apple Developer Program.
- Warning: The current app only runs on iOS 16.4.1 and will need to be updated using XCode.

## Improvements:

Due to my limited knowledge of transferring data to databases with IoT devices and the lack of support for the Pulse Sensor, this project can have some significant improvements. These improvements revolve mostly around correcting the EKG graph but there are also some improvements to be made for data transfer speeds and different sensors that better align with the project goal. 

Ideas for Improvement:
 - The first thing that could be improved is the pulse sensor reading capabilities. The library for the pulse sensor does not work properly and as such I had to create my own functions to record BPM and heartbeat strength.

 - The second thing that could be improved is the website to add functions to graph historical data instead of just reading it. I also would want to make the EKG graph on the website only graph the real time data coming in or graph a specific historical recording instead of it graphing the entire history of data but only displaying a specific section.

 - The third thing I would want to improve is the data transfer speed. Currently the data gets recorded faster than it can be transferred to the firebase database which makes the EKG graph even less accurate. I corrected this issue for the BPM so that works fine and it doesn’t affect the temperature and humidity readings, but I would really love to improve the EKG graph.

 - The fourth thing I would do is just switch the DHT sensor for a body temperature and blood oxygen sensor. This would be more in line with the project goal of patient/self-monitoring.
