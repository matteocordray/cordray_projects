# Code for the autonomous MMP5 robot

This folder details the code for the project, and consists of the following files:

```bash
├─── Robot_Contoller.ino
├─── PID.h
├─── FuzzyControl.h
```

## Robot_Controller.ino

This is the main `.ino` for the Arduino project. The Arduino IDE was used for the class because of it's ease of use and simplicity for all members of the class to get into mechatronics. Though alternatives such as PlatformIO, or Atmel Studio could have been used, the Arduino IDE was the most accessible and best for prototyping on a tight deadline.

Just like any Arduino project, the `setup()` and `loop()` functions are used to initialize the Arduino and run the main code. The `setup()` function is as followed:

```cpp
Serial.begin(115200); // Init Serial
attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onPress, RISING); // attach interrupt to start program with button press
pixy_cannon.init(PIXY_CANNON_ADDR); // Initialize cannon Pixy cam

left_motors.attach(MOTOR2_PIN,500,2000); // Init motor pins
right_motors.attach(MOTOR1_PIN,500,2000); // Init motor pins
cannon.attach(CANNON_PIN); // Init cannon servo
cannon.write(1515); // set the cannon to zero

left_motors.write(STOP); // Stop motors 
right_motors.write(STOP); // Stop motors

pixy_line.init(PIXY_LINE_ADDR); // Initialize line following Pixy cam
pixy_line.setLamp(1, 1); // Turn on the lamp for the line following Pixy for better low-light vision
pixy_line.changeProg("line"); // Make sure pixy is set to line following

pinMode(BUZZER_PIN, OUTPUT); // set buzzer pin as an output

Serial.println("time,m_x,steering"); // Set the header up for the necessary text files in the data logger


/* ------- Setup for Fuzzy controller ---------- */

Steer.setMidPoints(30, 77, 155, 231, 295); // L, LM, M, RM, R
Steer.setMidPointsV(-25, -10, 0, 10, 25); //  VN, N, Z, P, VP (-100, -25, 0, 25, 100)
Steer.setSingletons(1600, 1550, 1450, 1350, 1300); // FF, FS, Z, BS, BF

Throttle.setMidPoints(33, 46, 60, 70, 80); // 22 in, 1 ft, 6 in, 4 in, 2 in
Throttle.setMidPointsV(-25, -10, 0, 10, 25); // VN, N, Z, P, VP (-25, -10, 0, 10, 25)
Throttle.setSingletons(100, 50, 0, -50, -100); // FF, FS, Z, BS, BF
```