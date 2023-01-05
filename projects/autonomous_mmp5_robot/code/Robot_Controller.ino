// Name: Matteo Cordray & Sam Hassak
// Assignment: Final Project - Robot Controller

/* Include headers */
#include <Servo.h>
#include <Pixy2I2C.h>
#include "PID.h"
#include "FuzzyControl.h"

/* Define headers */
#define MOTOR1_PIN 9
#define MOTOR2_PIN 11
#define STOP 1450
#define SLOW 1525
#define FAST 1600
#define STOPBAND 15
#define CODESTOP 1 // barcode number 1 for stop
#define CODELEFT 9 // barcode number 9 for left turn
#define ALTCODELEFT 14 // alternate barcode number if 9 is not seen for left turn...
#define CODERIGHT 4 // barcode for number 4 for right turn
#define CODEOBJECTIVE 7 // barcode for number 7 for reaching objective
#define CODEBOSS 10 // barcode for number 10 for reaching the boss
#define BUTTON_PIN 2
#define CANNON_PIN 3
#define BUZZER_PIN 5
#define X_CENTER    (pixy_line.frameWidth / 2)
#define PIXY_LINE_ADDR 0x55
#define PIXY_CANNON_ADDR 0x53

/* Global Variables */
bool isRunning = true; // change to TRUE for testing purposes without the button press

Servo left_motors;
Servo right_motors;
Servo cannon;

Pixy2I2C pixy_cannon;
Pixy2I2C pixy_line;

PID Direction(10,0,0); // init PID control
Fuzzy Steer(0.4, 0.7, 30); // init Fuzzy Control for steering
Fuzzy Throttle(0.4, 0.7, 30); // init Fuzzy Control for throttle


static int counter = 0;
bool centered = false;
bool friendly = false;
bool enemy = false;
bool targetOnLeftSide = false; // boolean for whether the target is to the left or right of the robot
bool shotBoss = false;
bool ranOnce = false;

const long turnInterval = 2000;
unsigned long previousMillis = 0;
unsigned long objInterval = 500;

void setup() {
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
  
}

void onPress(){
  isRunning = !isRunning;
}

void loop() {
  if (isRunning) { // Main loop (button is pressed once to start)
    
    /* --------------- OBSERVE -------------------- */
    int8_t mainF;
    int32_t error_line;
    int left_dir;
    int right_dir;

    mainF = pixy_line.line.getMainFeatures(); // Get latest data from Line Following Pixy
    
    if (mainF <= 0) { // If no line was found or something else went wrong...
      right_motors.write(STOP);
      left_motors.write(STOP);
      return;
    }

    if (mainF&LINE_VECTOR) { // Otherwise, we found a vector...
      
      error_line = (int32_t) pixy_line.line.vectors->m_x1 - (int32_t) X_CENTER; // set the error
    
      // Perform PID calculations
      Direction.update(error_line);

      // Update both right and left wheels (should be opposite each other)
      left_dir = .5 * Direction.motor_signal;
      right_dir = -.5 * Direction.motor_signal;
      
      if(pixy_line.line.vectors->m_y0 > pixy_line.line.vectors->m_y1){ // If vector is pointing away from us...
        // Slow down if intersection is present so the pixy has enough time to register the event
        if(pixy_line.line.vectors->m_flags&LINE_FLAG_INTERSECTION_PRESENT){
          left_dir += SLOW;
          right_dir += SLOW;
        } else { // if no intersection is present, keep going
          left_dir += FAST;
          right_dir += FAST;
        }
      } else { // if the vector is pointing down...we need to go backwards (rare case scenario)

        pixy_line.line.reverseVector();
        
      }
      left_motors.write(left_dir); // write instruction to left motor
      right_motors.write(right_dir); // write instruction to right motor

      cannon.write(1515); // maintain position of the cannon servo
      cannon.detach();

    }

    if (mainF&LINE_INTERSECTION) {
      pixy_line.line.intersections->print();
    }

    if (mainF&LINE_BARCODE) {
    pixy_line.line.barcodes->print();
      // code 1 is stop sign
      if (pixy_line.line.barcodes->m_code == CODESTOP){ /* STOP SIGNAL */
        
        left_motors.write(STOP); // stop left motor
        right_motors.write(STOP); // stop right motor
        
        while(1){}; // stay in this loop forever. END OF PROGRAM
      
      } else if (pixy_line.line.barcodes->m_code == CODERIGHT){ /* RIGHT TURN */
        
        pixy_line.line.setNextTurn(-60); // -60 is a right turn
        pixy_line.line.setDefaultTurn(-60); // -60 is a right turn
      
      } else if (pixy_line.line.barcodes->m_code == CODELEFT || pixy_line.line.barcodes->m_code == ALTCODELEFT){ /* LEFT TURN */
        pixy_line.line.setNextTurn(60); // 60 is a left turn        
        pixy_line.line.setDefaultTurn(60); // 60 is a left turn
      
      } else if (pixy_line.line.barcodes->m_code == CODEOBJECTIVE){
        
        // If an objective is detected, move a little forward so that the objective is not seen again twice after engaging with the target.

        left_motors.write(FAST); // left motor goes faster
        right_motors.write(FAST); // right motor goes faster

        long currentMillis = millis();
        long previousMillis = currentMillis;
        while(currentMillis - previousMillis <= 1000){ // move pixy forward for one second
          currentMillis = millis();
        }
        
        left_motors.write(STOP); // stop left motor
        right_motors.write(STOP); // stop right motor
        
        searchForObjective(); // stay inside of reachedObjective until returned true

        mainF = pixy_line.line.getMainFeatures(); // Get latest data from Line Following Pixy
        pixy_line.setLamp(1, 1); // Turn on the lamp for the line following Pixy for better low-light vision
        if(enemy){
          enemy = false;
          while(mainF <= 0){
            mainF = pixy_line.line.getMainFeatures(); // Get latest data from Line Following Pixy to find the line again
            
            if(targetOnLeftSide){
              
              right_motors.write(1350);
              left_motors.write(1550);
              
            } else {
              
              right_motors.write(1550);
              left_motors.write(1350);
            
            }
            
          }

          targetOnLeftSide = false;
        
          right_motors.write(STOP);
          left_motors.write(STOP);
        } else if (friendly){
          friendly = false;
          while(mainF <= 0){
            mainF = pixy_line.line.getMainFeatures(); // Get latest data from Line Following Pixy to find the line again
            right_motors.write(1350);
            left_motors.write(1350);
          }
        
          right_motors.write(STOP);
          left_motors.write(STOP);
        }
        
      } else if (pixy_line.line.barcodes->m_code == CODEBOSS){
        
        right_motors.write(STOP);
        left_motors.write(STOP);
        
        searchForObjective(); // search for the boss
      }
        
    }

  }
}

void goToFriendly(){
  while(!centerOnTarget()){};
  friendly = true;
  tone(BUZZER_PIN, 3000);
  delay(1000); // TODO: Remove these
  noTone(BUZZER_PIN);
  delay(1000); // TODO: Remove these
  
}

void shootEnemy(){
  ranOnce = false;
  while(!aimAtTarget()){};
  enemy = true;
  pixy_line.setLamp(0, 0);  
}

bool aimAtTarget(){
  pixy_cannon.ccc.getBlocks(); // get blocks in the frame
  
  if(pixy_cannon.ccc.numBlocks > 0){
    Steer.filterPixy(pixy_cannon.ccc.blocks[0].m_x); // filter pixy data for x
    Steer.updateMembership(); // update membership for both positon and velocity
  
    // Set the degree Firing values accordingly and set crisp output
    int steering = Steer.getCrispOutput();

    if(counter > 0){
      if(steering <= (STOP + DEADBAND) && steering >= (STOP - DEADBAND)){
        counter++;
        if(counter >= 20){ // if the counter is above 10 values
          counter = 0;
          centered = true;
        }
      } else {
        counter = 0; // if the previous value is not within range, reset the counter. This is because we are looking for ten CONSECUTIVE values
        centered = false;
      }
    } else if (counter == 0){
      if(steering <= (STOP + DEADBAND) && steering >= (STOP - DEADBAND)){
        counter++;
      }
    }
    
    rotateMMP5(steering, 0);
    
    
  } else {
    right_motors.write(STOP);
    left_motors.write(STOP);
  }
  
  if(centered){

    if(!ranOnce){
      cannon.attach(CANNON_PIN);
      cannon.write(1800);
      previousMillis = millis();
      ranOnce = true;
    }

    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= 600){
      cannon.write(1515);
      cannon.detach();
      // CHANGE TO FALSE WHEN TESTING TRACKING CAPABILITIES
      return false; 
    }
    
    return false;
  } else {
    // TODO: redundant code
    return false;
  }
}

bool centerOnTarget(){
  pixy_cannon.ccc.getBlocks(); // get blocks in the frame
  
  if(pixy_cannon.ccc.numBlocks > 0){
    Steer.filterPixy(pixy_cannon.ccc.blocks[0].m_x); // filter pixy data for x
    Throttle.filterPixy(pixy_cannon.ccc.blocks[0].m_height); // filter pixy data for height
    Serial.print(millis());
    Serial.print(",");
    Serial.print(pixy_cannon.ccc.blocks[0].m_x);
    Serial.print(",");
    
    Steer.updateMembership(); // update membership for both positon and velocity
    Throttle.updateMembership(); // update membership for both position and velocity
  
    // Set the degree Firing values accordingly and set crisp output
    int steering = Steer.getCrispOutput();
    int throttle = Throttle.getCrispOutput();
    Serial.println(throttle);
    

    if(counter > 0){
      if((steering <= (STOP + DEADBAND) && steering >= (STOP - DEADBAND)) && (throttle <= STOPBAND && throttle >= -STOPBAND)){
        counter++;
        if(counter >= 20){ // if the counter is above 20 values
          counter = 0;
          centered = true;
        }
      } else {
        counter = 0; // if the previous value is not within range, reset the counter. This is because we are looking for ten CONSECUTIVE values
        centered = false;
      }
    } else if (counter == 0){
      if((steering <= (STOP + DEADBAND) && steering >= (STOP - DEADBAND)) && (throttle <= STOPBAND && throttle >= -STOPBAND)){
        counter++;
      }
    }
    
    rotateMMP5(steering, throttle);
    
    
  } else {
    right_motors.write(STOP);
    left_motors.write(STOP);
  }

  if(centered){
    right_motors.write(STOP);
    left_motors.write(STOP);
    centered = false;
    return true;
  } else {
    return false;
  }
}

void searchForObjective(){

  pixy_line.setLamp(0,0); // turn lamp off to save power
  pixy_cannon.ccc.getBlocks();
  
  previousMillis = millis();
  bool biggerBlock = false;
  while(!biggerBlock){ // while there are no blocks on the screen...
    pixy_cannon.ccc.getBlocks();
    
    if(pixy_cannon.ccc.numBlocks > 0){
      if(pixy_cannon.ccc.blocks[0].m_height > 10){
        biggerBlock = true;
      }
    }
    
    right_motors.write(1350);
    left_motors.write(1550);
    
    // determine if the target is on the left or right side
    unsigned long currentMillis = millis(); // Retrieve the current time
    if(currentMillis - previousMillis >= turnInterval){
      
      previousMillis = currentMillis; // reset clock
      targetOnLeftSide = true; // target is on the left side
      
    }
  }

  right_motors.write(STOP);
  left_motors.write(STOP);

  // Once done searching
  
  if(pixy_cannon.ccc.blocks[0].m_signature == 11){ // if the objective is a friendly...
    goToFriendly();
  
  } else if(pixy_cannon.ccc.blocks[0].m_signature == 10){ // if the objective is an enemy...
    shootEnemy();
  } else {

    shootEnemy();
    
  }
}

void rotateMMP5(int command, int throttle){
  
  // Positive command is to turn right, negative command is to turn left
  if(command > (STOP + 15)){
    right_motors.write(command + throttle);

    left_motors.write((STOP - (command - STOP)) + throttle);

  } else if(command < (STOP - 15)){
    right_motors.write(command + throttle);

    left_motors.write(((STOP - command) + STOP) + throttle);

  } else {
    right_motors.write(STOP + throttle);

    left_motors.write(STOP + throttle);

  }
  
}

/* EOF */
