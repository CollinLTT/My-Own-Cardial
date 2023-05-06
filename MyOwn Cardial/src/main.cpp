/*
CS596 SDSU
Collin Trump
Spring 2023

This portion of code is built to run on the ESP32 TTGO T-Display and uses the sensors DHT20 and SEN-11574 Pulse sensor.
The DHT20 uses I2C wiring.

*/

#include <Arduino.h>
#include <string>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <ezButton.h>
#include "time.h"
#include <DHT20.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Spare Change Crew"
#define WIFI_PASSWORD "pennypesofranc"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDrEZmbnxGXxQcky1jtl3Gs6HjTjrM3_yc"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "ctrump001@gmail.com"
#define USER_PASSWORD "admin1"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://myowncardial-default-rtdb.firebaseio.com/"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String bpmPath = "/BPM";
String ecgPath = "/ECG";
String timePath = "/timestamp";
String tempPath = "/temperature";
String humidityPath = "/humidity";

// Parent Node (to be updated in every loop)
String parentPath;

int timestamp;
FirebaseJson json;

const char* ntpServer = "pool.ntp.org";

int BPM = 0;
int ECG = 0;
int temp = 0;
DHT20 DHT;
uint8_t count = 0;

//  Variables
int PulseSensorPurplePin = 33;       // Pulse Sensor purple wire
int greenLED = 15;                   // Green LED
int redLED = 32;                      // Red LED 
unsigned long sendDataPrevMillis = 0;

ezButton button(17);                  // Button
unsigned long benchmark_millis;       // Benchmark for recording program time
unsigned long benchmark_led = 0;

int Signal;                           // Holds the incoming raw data from pulse sensor
int prevSignal;                       // Holds the previous loop's signal value
int Threshold = 2700;                 // Sets the threshold to count a beat
bool buttonPressed;


// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

//Calculates the BPM given the running time of the program
int calculateBPM(int nBPM, long int benchmark){
  int multiplier = 1;
  int currSec = (millis() - benchmark)/1000;
  if(currSec < 1){
    multiplier = 60;
  }
  else{
    multiplier = 60/currSec;
  }
  nBPM = nBPM * multiplier;
  return nBPM;
}

void setup(){

  DHT.begin();
  Serial.begin(9600);             // Set's up Serial Communication at certain speed.
  pinMode(redLED,OUTPUT);         // Red LED for heartbeat - initialization
  pinMode(greenLED,OUTPUT);       // Green LED for data transfer - initialization
  buttonPressed = false;          // Bool to check for pressed button


  //Connects to WiFi
  initWiFi();
  configTime(0, 0, ntpServer);

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the Firebase Real Time Database URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback;

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase auth and config
  Firebase.begin(&config, &auth);

  // Gets the user UID and prints '.' for each second waited
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update the database path with current UID
  databasePath = "/UsersData/";
  databasePath += uid;
  databasePath += "/readings";

  //Moves to loop
}

void loop(){

  button.loop();

  //Humidity and Temperature reading
  uint32_t start = micros();
  int status = DHT.read();
  uint32_t stop = micros();

  // While WiFi is connected and ready to transfer data, green LED is on
  if(buttonPressed == false){
    digitalWrite(greenLED,HIGH);
  }

  //Once button is pressed a timer starts and the green LED turns off
  if(button.isPressed()){

    Serial.println("Button pressed.");
    buttonPressed = true;
    benchmark_millis = millis();
    digitalWrite(greenLED,LOW);
  }

  // Checks if 30 seconds have passed since button was pressed
  if(((millis() - benchmark_millis) >= 30000) && buttonPressed == true){
      Serial.println("-----------------------------------------------");
      Serial.println("30 seconds elapsed.");
      Serial.println("-----------------------------------------------");
      buttonPressed = false;
      BPM = 0;
    }

  // If button has been pressed, records BPM & Pulse strength & temperature
  if(buttonPressed == true){

    Signal = analogRead(PulseSensorPurplePin);

    //Prints the current readings in the console
    Serial.print("Pulse strength: ");       
    Serial.println(analogRead(PulseSensorPurplePin));                    
    Serial.print("BPM: ");       
    Serial.println(calculateBPM(BPM, benchmark_millis));   
    Serial.print("Temperature: ");       
    Serial.println(DHT.getTemperature());   
    Serial.print("Humidity: ");       
    Serial.println(DHT.getHumidity()); 

    sendDataPrevMillis = millis();

    //Get current timestamp
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);

    parentPath= databasePath;
    parentPath += "/";
    parentPath += String(timestamp);

    // Sends the data to the firebase database 
    json.set(bpmPath.c_str(), String(calculateBPM(BPM, benchmark_millis)));
    json.set(ecgPath.c_str(), String(analogRead(PulseSensorPurplePin)));
    json.set(tempPath.c_str(), String(DHT.getTemperature()));
    json.set(humidityPath.c_str(), String(DHT.getHumidity()));
    json.set(timePath, String(timestamp));
    Serial.printf("Sending json data... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "[sent]" : fbdo.errorReason().c_str());
    
    // Checks to make sure enough time has passed between heart beats then adjusts BPM and red LED
    if(Signal < Threshold){
      prevSignal = Signal;
    }

    if(prevSignal > Signal){
      delay(50);
    }
    else{

      if(Signal > Threshold){             // If the signal is above the threshold the red LED turns on and adds a count to BPM
        if(millis() - benchmark_led > 300){
          benchmark_led = millis();
          digitalWrite(redLED,HIGH);
          delay(50);
        }
        
          digitalWrite(redLED,LOW);
        
      BPM += 1;
      prevSignal = Signal;
    }

    delay(50);
    }

  }
    
}