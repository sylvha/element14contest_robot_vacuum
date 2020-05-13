// Include required packages

// As mentioned in the video, you can download the LCD library 
// from https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home 
// and follow this  video https://www.youtube.com/watch?v=xVC0X_PE_XE 
// to configure your LCD screen 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/sleep.h>

// Initialise the LCD 
LiquidCrystal_I2C lcd(0x3F, 2,1,0,4,5,6,7,3,POSITIVE);


// On/off button pin
const int buttonPin = 2;
bool buttonPressed = false;

// Set up the pins, these are constant
const int leftMotorEnablePin = 3;       // Pins for the left wheel
const int leftMotorPositivePin = 4;
const int leftMotorNegativePin = 5; 

const int rightMotorEnablePin = 6;      // Pins for the right wheel
const int rightMotorNegativePin = 7;
const int rightMotorPositivePin = 8;

// Pins for the ultrasonic sensor
const int trigPin = 9;      
const int echoPin = 10;

// Pin for the sweepers        
const int motorPin = 11; 

// Variables for the ultrasonic sensor
long duration;                
int distance;

// Variable for robot to turn
int angle_to_turn;

void setup() {
  // Set the on/off button as an input
  pinMode(buttonPin, INPUT);
    Serial.begin(9600);     // Serial monitor for debugging

  // Set the trigger pin as an output
  pinMode(trigPin, OUTPUT);
  
  // Set the echo pin as an input
  pinMode(echoPin, INPUT); 
  
  // Initialise the lcd - 16 columns and 2 rows
  lcd.begin(16,2);

  // Set the wheels as outputs, both the enable pins and 2 sides of each motor
  pinMode(leftMotorEnablePin, OUTPUT);
  pinMode(leftMotorNegativePin, OUTPUT);
  pinMode(leftMotorPositivePin, OUTPUT);
  
  pinMode(rightMotorEnablePin, OUTPUT);
  pinMode(rightMotorNegativePin, OUTPUT);
  pinMode(rightMotorPositivePin, OUTPUT);
  
  // Set motor pin as an output
  pinMode(motorPin, OUTPUT);
}

/*
 *  Displays a welcome message on the LCD
 */
void welcomeMsg(){       
    lcd.clear();                  // Clear the screen
    lcd.print("Welcome to RV!");  // Welcome user to Robot Vacuum 
    lcd.setCursor(0,1);           // Set the cursor to the first character, second row
    lcd.print("Cleaning in ...");
    delay(2000);                  // Display for 2 seconds
    lcd.clear();
    lcd.print("3...");            // Display 3,2,1 with one second delays
    delay(1000);
    lcd.clear();
    lcd.print("2...");
    delay(1000);
    lcd.clear();
    lcd.print("1...");
    delay(1000);
    lcd.clear();
    lcd.print("GO!");

}

/*
 *  Displays a welcome message on the LCD
 */
void goodbyeMsg(){       
    lcd.clear();                  // Clear the screen
    lcd.print("GOODBYE!");        // Goodbye message 
}
void wakeUpNow() {
  //  Code to run immediately after wakeup
  // Purely optional
}

void goToSleep() {
//  Serial.print("state of pin inside sleep ");   // Debugging
//  Serial.println(digitalRead(buttonPin));
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(0, wakeUpNow, LOW);
  sleep_mode();
  sleep_disable();         
  detachInterrupt(0);

  // Decide whether to display the welcome message or goodbye message
  if (!buttonPressed)
  {
    welcomeMsg();
  }else
  {
    goodbyeMsg();
  }
  buttonPressed = true;

}

/*
 * Moves the car forwards for a specified number of milliseconds
 */
void moveForwards(int delayTime){
  enableMotors();  
  
  // set the left and right motors to spin forwards
  digitalWrite(leftMotorPositivePin, HIGH);
  digitalWrite(leftMotorNegativePin, LOW);

  digitalWrite(rightMotorPositivePin, HIGH);
  digitalWrite(rightMotorNegativePin, LOW);
   
  delay(delayTime);
  disableMotors(); // disable so they don't just keep spinning forwards
}


/*
 * Turn the car to the right for a specified number of milliseconds
 */
void turnRight(int delayTime){
  enableMotors();
  
  digitalWrite(leftMotorPositivePin, HIGH);
  digitalWrite(leftMotorNegativePin, LOW);

  digitalWrite(rightMotorPositivePin, LOW);
  digitalWrite(rightMotorNegativePin, HIGH);
  
  delay(delayTime);
  disableMotors();
}


/*
 * Enable the wheel motors
 */
void enableMotors(){
  analogWrite(leftMotorEnablePin, 150); // set a speed (out of 255) by sending PWM signal
  analogWrite(rightMotorEnablePin, 150); 
  delay(1);
  }
  
/*
 * Disable the wheel motors
 */
void disableMotors(){
  analogWrite(leftMotorEnablePin, 0);
  analogWrite(rightMotorEnablePin, 0);
  delay(1);
}


void loop(){
  goToSleep();
//  Serial.print("state of button press ");
//  Serial.println(buttonPressed);

  // While the button is ON, keep running the vacuum robot
  while (digitalRead(buttonPin) && buttonPressed ) {

//    Serial.print("state of pin ");
//    Serial.println(digitalRead(buttonPin));

    if (!digitalRead(buttonPin)){
      buttonPressed = !buttonPressed;
      }

    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculating the distance
    distance= duration*0.034/2;
    
    lcd.clear();
    lcd.setCursor(0,0);         // Set the cursor to the first character, first row
    lcd.print("Distance: ");    // Prints string "Distance" on the LCD
    lcd.print(distance);        // Prints the distance value from the sensor
    lcd.print(" cm");
    delay(10);

    // Sweeper motors 
    analogWrite(motorPin, 200); // Motor at 150/255 of full speed
    delay(10);
    analogWrite(motorPin, 190);; // Motor at 250/255 of full speed
    delay(10);

    // If we are too close to a wall/obstacle then turn right
    if (distance > 15)
    {
      moveForwards(500); 
    }
    else
    {
      angle_to_turn = random(4,6)*200;
      turnRight(angle_to_turn); 
    }  
        
  }
}
