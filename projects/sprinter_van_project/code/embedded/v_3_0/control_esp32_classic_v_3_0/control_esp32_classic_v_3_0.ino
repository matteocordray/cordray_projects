/*


 ------------------------------------------------
|  Iconic Sprinter Van ESP32 Controller Code     |
|  Created by: Matteo Cordray                    |
|  Date last updated: 12/15/2022                 |
 ------------------------------------------------

@file control_esp32_classic_v_3_0.ino

/**
 * ***Update - 15DEC22***
 * - Moved copy of this file to `cordray_projects` repository
 * - Remove commented out code
 */

/**
 * ***Update - 8JUL22***
 * - Commented out extra temperature variables
 * 
 */

/**
 * Update - 16AUG22
 * - Replaced readtemperature function with extra temperature variable
 * 
 */

/**
 * Update - 9SEP22
 * - Replaced all HIGHs to LOW and LOW to HIGH because SainSmart Relay board is Active-Low
 * 
 */

#include <BluetoothSerial.h>
#include "Adafruit_MCP23017.h"
#include <Smoothed.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 14

#define green_counter_size 50
#define red_counter_size 50
#define blue_counter_size 50
#define main_counter_size 200

Adafruit_MCP23017 mcp;    // Instantiate mcp object

#define BLINDS_SIZE 6
uint8_t mac_addr[6] = {0xAC, 0x67, 0xB2, 0x39, 0x52, 0x14};

Smoothed <int> RGBController_green;    // Readings from RGB controller for smoother readings
Smoothed <int> RGBController_red;
Smoothed <int> RGBController_blue;
Smoothed <int> RGBController_main;

int buPos[BLINDS_SIZE] = {0, 2, 4, 8, 10, 12};
int bdPos[BLINDS_SIZE] = {1, 3, 5, 9, 11, 13};

// Pins
const uint8_t tempPin = 14;
const uint8_t mainLightsPin = 27;
const uint8_t starLightsPin = 26;
const uint8_t coolPin = 32;
const uint8_t heatPin = 12;
const uint8_t lowPin = 13;
const uint8_t medPin = 25;
const uint8_t highPin = 33;

const uint8_t mainLightsINPUT = 18;
const uint8_t redINPUT = 16;
const uint8_t blueINPUT = 17;
const uint8_t greenINPUT = 5;
const uint8_t ledpowerPin = 15;

// the number of color pins
const int redPin = 2;
const int greenPin = 0;
const int bluePin = 4;

volatile int prev_time_red = 0;
volatile int prev_time_green = 0;
volatile int prev_time_blue = 0;
volatile int prev_time_main = 0;

// setting PWM properties
const int freq = 1000;
const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;
const int mainChannel = 3;
const int ledChannel = 4;
const int resolution = 8;

// PWM Read values
int redINPUTVAL = 0;
int greenINPUTVAL = 0;
int blueINPUTVAL = 0;
int mainINPUTVAL = 0;

// PWM old Read values
int previousRedVal = 0;
int previousGreenVal = 0;
int previousBlueVal = 0;
int previousMainVal = 0;

// Counters for PWM readings
int green_counter = 0;
int red_counter = 0;
int blue_counter = 0;
int main_counter = 0;

// const int ledPowerPin = 0;

// Brightness levels
int LEDPower = 0;
double percent = 0;

bool ledPower, mainPower = false;
int red, green, blue = 0;
int mainL = 0;

// Temperature data

int desiredTemp = 70;
uint8_t currentTemp = 70;

String mode = "OFF";
String fanSpeed = "LOW";

bool heating = false;
bool cooling = false;
bool acOFF = false;
bool allBlindsUp = false;
bool allBlindsDown = false;


// Temperature sensor for van (separate from the A/C unit temperature sensor)
DHT dht(DHTPIN, DHTTYPE);

// old temperature variables
/* int Vo;
float R1 = 10000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
*/
unsigned long previousMillis = 0;
const long interval = 2000;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
char *pin = "0000"; //<- standard pin would be provided by default

void coolAC(){
  
  if(!cooling){
    cooling = true;
    heating = false;
    acOFF = false;
    Serial.println("Cool AC!");
    digitalWrite(coolPin, HIGH);
    digitalWrite(heatPin, LOW);
    if(fanSpeed == "LOW"){
      digitalWrite(lowPin, HIGH);
      digitalWrite(medPin, LOW);
      digitalWrite(highPin, LOW);
    } else if (fanSpeed == "MED"){
        digitalWrite(lowPin, LOW);
        digitalWrite(medPin, HIGH);
        digitalWrite(highPin, LOW);      
    } else if (fanSpeed == "HIGH"){
        digitalWrite(lowPin, LOW);
        digitalWrite(medPin, LOW);
        digitalWrite(highPin, HIGH);
    }
  }
  
}

void heatAC(){
  if(!heating){
    cooling = false;
    heating = true;
    acOFF = false;
    Serial.println("Heat AC!");
    digitalWrite(coolPin, LOW);
    digitalWrite(heatPin, HIGH);
    if(fanSpeed == "LOW"){
      digitalWrite(lowPin, HIGH);
      digitalWrite(medPin, LOW);
      digitalWrite(highPin, LOW);
    } else if (fanSpeed == "MED"){
        digitalWrite(lowPin, LOW);
        digitalWrite(medPin, HIGH);
        digitalWrite(highPin, LOW);      
    } else if (fanSpeed == "HIGH"){
        digitalWrite(lowPin, LOW);
        digitalWrite(medPin, LOW);
        digitalWrite(highPin, HIGH);
    }
  }
}

void offAC(){
  
  if(!acOFF){
    acOFF = true;
    cooling = false;
    heating = false;
    Serial.println("Turn AC off!");
    digitalWrite(coolPin, LOW);
    digitalWrite(heatPin, LOW);
    digitalWrite(lowPin, LOW);
    digitalWrite(medPin, LOW);
    digitalWrite(highPin, LOW);
  }
  
}

void handleReceivedMessage(char *msg)
{
  String message = String(msg);
  Serial.print("Msg: ");
  Serial.println(msg);
  
  if(strcmp(msg, "S1") == 0)              // STARLIGHTS
  {
    // Serial.println("Starlight On!");
    digitalWrite(starLightsPin, LOW); // CHANGE TO LOW FOR SAINSMART BOARD
  } else if (strcmp(msg, "S0") == 0) {
    // Serial.println("Starlight Off!");
    digitalWrite(starLightsPin, HIGH); // CHANGE TO HIGH FOR SAINSMART BOARD
  } else if (strcmp(msg, "M0") == 0) {    // MAIN LIGHTS
    // Serial.println("Main Lights off!");
    mainPower = false;
    ledcWrite(mainChannel, 0); // Turn Main Lights off
    
  } else if (strcmp(msg, "M1") == 0) {
    // Serial.println("Main Lights on!);
    mainPower = true;
    ledcWrite(mainChannel, mainL); // Turn Main Lights on
    
  } else if (message.substring(0,2) == "MD") {
    // Serial.println("Main Lights on!);
    mainL = message.substring(2).toInt();
    if(mainPower){
      ledcWrite(mainChannel, mainL); // Change brightness for Main Lights
    }
    
    
  } else if (strcmp(msg, "L0") == 0) {    // LED LIGHTS
    // Serial.println("LED Lights off!");
    ledPower = false;         // Set power to false for no more changes
    ledcWrite(redChannel, 0);
    ledcWrite(greenChannel, 0);
    ledcWrite(blueChannel, 0);
    
  } else if (strcmp(msg, "L1") == 0) {
    // Serial.println("LED Lights on!);
    ledPower = true;          // Set power to true to allow changes
    ledcWrite(redChannel, int(red*percent));
    ledcWrite(greenChannel, int(green*percent));
    ledcWrite(blueChannel, int(blue*percent));
  } else if (message.substring(0,2) == "LB") { // SET BRIGHTNESS FOR LED LIGHTS
    // Serial.println("LED Lights Brightness");
    LEDPower = message.substring(2).toInt();
    Serial.print("Brightness for LEDs set to: ");
    Serial.println(LEDPower);
    // ledcWrite(ledChannel, LEDPower);
    percent = map(LEDPower, 0, 255, 0, 100) / 100.0;
    Serial.print("Percentage: ");
    Serial.println(percent);

    // Output color to LED strip
      
    if(ledPower){
      ledcWrite(redChannel, int(red*percent));
      ledcWrite(greenChannel, int(green*percent));
      ledcWrite(blueChannel, int(blue*percent));
    } else {
      ledcWrite(redChannel, 0);
      ledcWrite(greenChannel, 0);
      ledcWrite(blueChannel, 0);
    }
    
  } else if (message.substring(0,2) == "LC") { // SET NEW LED COLOR
    // Serial.println("New LED Color!);

    String value = message.substring(2);
    int firstcomma = value.indexOf('.');
    red = value.substring(0,firstcomma).toInt();
    Serial.print(red);
    Serial.println();
    int secondcomma = value.indexOf(',');
    green = value.substring(firstcomma+1,secondcomma).toInt();
    Serial.print(green);
    Serial.println();
    blue = value.substring(secondcomma+1).toInt();
    Serial.print(blue);
    Serial.println();

    // Output color to LED strip
      
    if(ledPower){
      ledcWrite(redChannel, int(red*percent));
      ledcWrite(greenChannel, int(green*percent));
      ledcWrite(blueChannel, int(blue*percent));
    } else {
      ledcWrite(redChannel, 0);
      ledcWrite(greenChannel, 0);
      ledcWrite(blueChannel, 0);
    }
    
  } else if (message.substring(0,1) == "U") {    // BLINDS UP
    // Serial.println("Blind up!");
    if (message.substring(1) == "A"){
      allBlindsUp = true;
      allBlindsDown = false;
      Serial.println("All Blinds up!");
      Serial.println(sizeof(buPos));
      
      for (int i = 0; i < BLINDS_SIZE; i++) {
        mcp.digitalWrite(bdPos[i], HIGH); // CHANGE TO HIGH FOR SAINSMART BOARD
        mcp.digitalWrite(buPos[i], LOW);
      }
      
    } else {
      int blindNum = message.substring(1).toInt();
      
      Serial.print("Blind ");
      Serial.print(String(blindNum));
      Serial.println(" up!");

      // Stop blind moving down
      mcp.digitalWrite(bdPos[blindNum - 1], HIGH);
      // Move blind up
      mcp.digitalWrite(buPos[blindNum - 1], LOW);
      
      
      
    }
    
    
  } else if (message.substring(0,1) == "D") {       // BLINDS DOWN
    
    if (message.substring(1) == "A"){
      allBlindsDown = true;
      allBlindsUp = false;
      Serial.println("All Blinds down!");
      Serial.println(sizeof(bdPos));
      
      for (int i = 0; i < BLINDS_SIZE; i++) {
        mcp.digitalWrite(buPos[i], HIGH); 
        mcp.digitalWrite(bdPos[i], LOW); // CHANGE TO LOW FOR SAINSMART BOARD
      }
      
    } else {
      // Serial.println("Blind down!);
      int blindNum = message.substring(1).toInt();
      Serial.print("Blind ");
      Serial.print(String(blindNum));
      Serial.println(" down!"); 

      // Stop blind moving up
      mcp.digitalWrite(buPos[blindNum - 1], HIGH);
      // Move blind down
      mcp.digitalWrite(bdPos[blindNum - 1], LOW); // CHANGE TO LOW FOR SAINSMART BOARD
      
                                                 
    }
                                                     
    
     
  } else if (message.substring(0,1) == "Z") {       // BLINDS STOP
    
    if (message.substring(1) == "A") {
      Serial.println("All blinds stop!");
      for (int i = 0; i < BLINDS_SIZE; i++) {
        mcp.digitalWrite(buPos[i], HIGH); // CHANGE TO HIGH FOR SAINSMART BOARD
        mcp.digitalWrite(bdPos[i], HIGH);
      }
      
    } else {
      // Serial.println("Blind stop!);
      int blindNum = message.substring(1).toInt();
  
      mcp.digitalWrite(buPos[blindNum - 1], HIGH); // CHANGE TO HIGH FOR SAINSMART BOARD
      mcp.digitalWrite(bdPos[blindNum - 1], HIGH);
      
      Serial.print("Blind ");
      Serial.print(String(blindNum));
      Serial.println(" stopping!");  
    }
    
  } else if (strcmp(msg, "B1") == 0) {    // COOL
    
    mode = "COOL";      // Set to COOL

    // Write to Pins
    coolAC();

  } else if (strcmp(msg, "B2") == 0) {    // HEAT
    
    mode = "HEAT";      // Set to HEAT

    // Write to Pins
    heatAC();
  } else if (strcmp(msg, "B3") == 0) {    // OFF
    
    mode = "OFF";      // Set to OFF

    offAC();
  } else if (strcmp(msg, "F1") == 0 && mode != "OFF") {    // LOW
    fanSpeed = "LOW";
    digitalWrite(lowPin, HIGH);
    digitalWrite(medPin, LOW);
    digitalWrite(highPin, LOW);
  } else if (strcmp(msg, "F2") == 0 && mode != "OFF") {    // MEDIUM
    fanSpeed = "MED";
    digitalWrite(lowPin, LOW);
    digitalWrite(medPin, HIGH);
    digitalWrite(highPin, LOW);
  } else if (strcmp(msg, "F3") == 0 && mode != "OFF") {    // HIGH
    fanSpeed = "HIGH";
    digitalWrite(lowPin, LOW);
    digitalWrite(medPin, LOW);
    digitalWrite(highPin, HIGH);
  } else if (message.substring(0,1) == "E") {    // HIGH
    Serial.print("Change desired temperature to: ");
    desiredTemp = message.substring(1).toInt();
    Serial.println(desiredTemp);
  }
  else
  {
    Serial.println("INVALID");
    // etc
  }
}

void handleSerial()
{
    const int BUFF_SIZE = 32; // make it big enough to hold your longest command
    static char buffer[BUFF_SIZE+1]; // +1 allows space for the null terminator
    static int length = 0; // number of characters currently in the buffer

    if(SerialBT.available())
    {
        char c = SerialBT.read();
        if((c == '\r') || (c == '\n'))
        {
            // end-of-line received
            if(length > 0)
            {
                handleReceivedMessage(buffer);
            }
            length = 0;
        }
        else
        {
            if(length < BUFF_SIZE)
            {
                buffer[length++] = c; // append the received character to the array
                buffer[length] = 0; // append the null terminator
            }
            else
            {
                // buffer full - discard the received character
            }
        }
    }
}



void checkTemperature(int ctmp){
  // TODO: Code to check temperature
  
  if(mode == "COOL"){
    if(ctmp > desiredTemp + 2){     // +1 for calculating error | TODO: smooth out readings
      coolAC();
    } else if (ctmp < desiredTemp - 2) {
      offAC();
    }
  } else if(mode == "HEAT"){
    if(ctmp < desiredTemp + 2){     // +1 for calculating error | TODO: smooth out readings
      heatAC();
    } else if (ctmp > desiredTemp - 2) {
      offAC();
    }
  } 
  // else if(mode == "OFF") {
  //   offAC();
  // }

}

void risingRED(){
  attachInterrupt(redINPUT, fallingRED, FALLING);
  prev_time_red = micros();
}

void fallingRED(){
  attachInterrupt(redINPUT, risingRED, RISING);
  redINPUTVAL = micros()- prev_time_red;
  redINPUTVAL = map(redINPUTVAL, 0, 1023, 0, 255);  //Scales 0-1023 to 0-255 (or whatever you want)
  RGBController_red.add(redINPUTVAL);
  red_counter++;
  if(red_counter > red_counter_size){
    if(RGBController_red.get() >= (previousRedVal + 2) || RGBController_red.get() <= (previousRedVal - 2)){
      previousRedVal = RGBController_red.get(); // Set new value
      if(previousRedVal != red){
        red = previousRedVal;
        // Output color to LED strip
        // if(ledPower){
          ledcWrite(redChannel, int(red*percent));
        // }
      }
    }
  }
  
  //Serial.println(redINPUTVAL,DEC); //Prints as decimal, with line feed
}

void risingGREEN(){
  attachInterrupt(greenINPUT, fallingGREEN, FALLING);
  prev_time_green = micros();
}

void fallingGREEN(){
  attachInterrupt(greenINPUT, risingGREEN, RISING);
  greenINPUTVAL = micros()- prev_time_green;
  greenINPUTVAL = map(greenINPUTVAL, 0, 1023, 0, 255);  //Scales 0-1023 to 0-255 (or whatever you want)
  RGBController_green.add(greenINPUTVAL);
  green_counter++;
  if(green_counter > green_counter_size){
    if(RGBController_green.get() >= (previousGreenVal + 2) || RGBController_green.get() <= (previousGreenVal - 2)){
      previousGreenVal = RGBController_green.get(); // Set new value
      if(previousGreenVal != green){
        green = previousGreenVal;
        // Output color to LED strip
        // if(ledPower){
          ledcWrite(greenChannel, int(green*percent));
        // }
      }
    }
  }
}

void risingBLUE(){
  attachInterrupt(blueINPUT, fallingBLUE, FALLING);
  prev_time_blue = micros();
}

void fallingBLUE(){
  attachInterrupt(blueINPUT, risingBLUE, RISING);
  blueINPUTVAL = micros()- prev_time_blue;
  blueINPUTVAL = map(blueINPUTVAL, 0, 1023, 0, 255);  //Scales 0-1023 to 0-255 (or whatever you want)
  RGBController_blue.add(blueINPUTVAL);
  blue_counter++;
  if(blue_counter > blue_counter_size){
    if(RGBController_blue.get() >= (previousBlueVal + 2) || RGBController_blue.get() <= (previousBlueVal - 2)){
      previousBlueVal = RGBController_blue.get(); // Set new value
      if(previousBlueVal != blue){
        blue = previousBlueVal;
        // Output color to LED strip
        // if(ledPower){
          ledcWrite(blueChannel, int(blue*percent));
        // }
      }
    }
  }
  
  //Serial.println(redINPUTVAL,DEC); //Prints as decimal, with line feed
}

void risingMAIN(){
  attachInterrupt(mainLightsINPUT, fallingMAIN, FALLING);
  prev_time_main = micros();
}

void fallingMAIN(){
  attachInterrupt(mainLightsINPUT, risingMAIN, RISING);
  mainINPUTVAL = micros() - prev_time_main;
  mainINPUTVAL = map(mainINPUTVAL, 0, 2000, 0, 255);
  RGBController_main.add(mainINPUTVAL);
  main_counter++;
  if(main_counter > main_counter_size){
    if((RGBController_main.get() >= (previousMainVal + 2) || RGBController_main.get() <= (previousMainVal - 2)) && RGBController_main.get() <= 255){
      previousMainVal = RGBController_main.get(); // Set new value
      if(previousMainVal != mainL){
        mainL = previousMainVal;
        // Output color to LED strip
        ledcWrite(mainChannel, mainL);
        
      }
    }
  }

}

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client Connected");
  }
 
  if(event == ESP_SPP_CLOSE_EVT ){
    Serial.println("Client disconnected");
    // Turn all AC outputs off
    digitalWrite(lowPin, LOW);
    digitalWrite(medPin, LOW);
    digitalWrite(highPin, LOW);
    digitalWrite(heatPin, LOW);
    digitalWrite(coolPin, LOW);
  }
}


void setup() {
  esp_base_mac_addr_set(mac_addr);  //Set mac address for ESP32 to any device that receives this code.
  Serial.begin(115200);
  SerialBT.begin("ICONIC CONTROLLER"); //Bluetooth device name
  SerialBT.setPin(pin);
  Serial.println("The device started, now you can pair it with bluetooth!");

  SerialBT.register_callback(callback);

  // mySensor.begin(SMOOTHED_AVERAGE, 10);
  dht.begin();
  
  // Configure output pins for lights and temperature
  pinMode(tempPin, INPUT);
  pinMode(coolPin, OUTPUT);
  pinMode(heatPin, OUTPUT);
  pinMode(lowPin, OUTPUT);
  pinMode(medPin, OUTPUT);
  pinMode(highPin, OUTPUT);
  pinMode(starLightsPin, OUTPUT);
  pinMode(mainLightsINPUT, INPUT);
  pinMode(redINPUT, INPUT);
  pinMode(blueINPUT, INPUT);
  pinMode(greenINPUT, INPUT);

  // ATTACH INTERRUPTS TO READ ADC VALUES FOR RGB AND MAINLIGHTS
  attachInterrupt(redINPUT, risingRED, RISING); //red
  attachInterrupt(greenINPUT, risingGREEN, RISING); //green
  attachInterrupt(blueINPUT, risingBLUE, RISING); //blue
  attachInterrupt(mainLightsINPUT, risingMAIN, RISING); //main

  // DEFAULT MODE FOR AC
  // digitalWrite(lowPin, HIGH);
  
  // configure LED PWM functionalitites
  ledcSetup(redPin, freq, resolution); // Red
  ledcSetup(greenPin, freq, resolution); // Green
  ledcSetup(bluePin, freq, resolution); // Blue
  ledcSetup(ledpowerPin, freq, resolution); // RGB Power
  ledcSetup(mainLightsPin, freq, resolution); // Main Lights

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(redPin, redChannel); // Red
  ledcAttachPin(greenPin, greenChannel); // Green
  ledcAttachPin(bluePin, blueChannel); // Blue
  ledcAttachPin(ledpowerPin, ledChannel); // Led Power
  ledcAttachPin(mainLightsPin, mainChannel); // Main Lights

  // Start blinds controller
  mcp.begin();            // "Start" the mcp object

  // Start RGB sensor smoothing
  RGBController_green.begin(SMOOTHED_AVERAGE, green_counter_size);
  RGBController_red.begin(SMOOTHED_AVERAGE, red_counter_size);
  RGBController_blue.begin(SMOOTHED_AVERAGE, blue_counter_size);
  RGBController_main.begin(SMOOTHED_AVERAGE, main_counter_size);

  // Configure all pins for output
  for(int i = 0; i <= 13; i++) {
    mcp.pinMode(i, OUTPUT);
  }
}

void loop() {
//  Serial.print("Smoothed Main: ");
//  Serial.print(RGBController_main.get());
//  Serial.print("\tActual input: ");
//  Serial.print(mainINPUTVAL);
//  Serial.print("\tPrevious Main: ");
//  Serial.print(previousMainVal);
//  Serial.print("\tActual Main Value: ");
//  Serial.println(mainL);

//  Serial.print("Red: ");
//  Serial.print(RGBController_red.get());
//  Serial.print("\tGreen: ");
//  Serial.print(RGBController_green.get());
//  Serial.print("\tBlue: ");
//  Serial.print(RGBController_blue.get());
//  Serial.print("\tMain: ");
//  Serial.println(RGBController_main.get());
  
  handleSerial();
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    
    float currentSensorValue = analogRead(tempPin);
    
    // Add the new value to both sensor value stores
    // mySensor.add(currentSensorValue);
    
    // Vo = analogRead(tempPin);
    // Vo = mySensor.get();
    // R2 = R1 * (1023.0 / (float)Vo - 1.0);
    // logR2 = log(R2);
    // T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
    // T = T - 273.15;
    // T = (T * 9.0)/ 5.0 + 32.0;
    // T = dht.readTemperature(true);
    int newTemp = dht.readTemperature(true);
    checkTemperature(currentTemp);

    if(newTemp != currentTemp){

      currentTemp = newTemp;      // Set new temperature
      
      Serial.print("Temperature: "); 
      Serial.print(currentTemp);
      Serial.println(" F"); 
      
      //SerialBT.write('T' + currentTemp + '\n');
      SerialBT.write(currentTemp);
      checkTemperature(currentTemp);
      // SerialBT.write(13); // \n character
      // SerialBT.write('\n');
    }
    
  }
  
}
