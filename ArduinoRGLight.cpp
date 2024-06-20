#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h>
#include <NewTone.h>
#include <TimerOne.h>

// Pin configuration for the LCD
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

// Pins for the ultrasonic sensor
const int trigPin = 11;
const int echoPin = 12;

// Create an instance of the Servo library
Servo myServo;

// Pins for the LEDs and Buzzer
const int greenLEDPin = 9;
const int redLEDPin = 10;
const int buzzerPin = 22;

// Pin for the button with interrupt support
const int buttonPin = 3;

// Variable to store the remaining time
volatile int remainingTime = 60;

// Flag to indicate the game state (started or not)
volatile bool gameStarted = false;

// Variable to track the current light state (red or green)
volatile bool currentLightState = true; // Start with red light

// Global variable to store previous distance
volatile float previousDistance = 0.0;

// Tolerance threshold to discard small changes
const float distanceThreshold = 6.0; // Adjust as needed

// Declare startTime as a static variable within the loop() function
static unsigned long startTime = 0;
static unsigned long sensorStartTime = 0;

void buttonInterrupt();
void checkMotionDuringRedLight();
void updateRemainingTime();

void setup() {
    // Set up the LCD columns and rows
    lcd.begin(16, 2);

    // Set the initial remaining time on the LCD
    lcd.print("Time Left: ");
    lcd.print(remainingTime);

    // Set LED and Buzzer pins as outputs
    pinMode(greenLEDPin, OUTPUT);
    pinMode(redLEDPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);

    // Attach interrupt to the button pin
    attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING);

    // Servo setup
    myServo.attach(13); // Use the pin connected to the servo's signal wire

    // Ultrasonic sensor setup
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Set up Timer1 with a 1-second interval
    Timer1.initialize(1000000); // 1 second = 1,000,000 microseconds
    Timer1.attachInterrupt(updateRemainingTime);
}

float readDistance() {
    // Trigger the ultrasonic sensor to send a pulse
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the time it takes for the pulse to return
    float duration = pulseIn(echoPin, HIGH);

    // Convert the time to distance in centimeters
    float distance = (duration * 0.0343) / 2;

    return distance;
}

bool checkGameOutcome();
void loop() {
    if (gameStarted) {
        // Control Servo based on currentLightState
        if (remainingTime > 0) {
            if (currentLightState) {
                // Red light period
                myServo.write(0); // Rotate to 0 degrees for red light
                checkMotionDuringRedLight();
                NewTone(buzzerPin, 1000, 500); // Buzzer sound during red light

                // Introduce a 0.5-second delay when the sensor starts sensing during the red phase
                if (millis() - sensorStartTime >= 500) {
                    checkMotionDuringRedLight();
                }
            } else {
                // Green light period
                myServo.write(180); // Rotate to 180 degrees for green light
                NewTone(buzzerPin, 1500, 500); // Buzzer sound during green light
            }
        }

        // Control LEDs based on game state and remaining time
        if (remainingTime > 0) {
            if (currentLightState) {
                // Red light period
                digitalWrite(greenLEDPin, LOW);
                digitalWrite(redLEDPin, HIGH);

                // Switch to green light after 3 seconds
                if (millis() - startTime >= 3000) {
                    currentLightState = false;
                    startTime = millis();
                }
            } else {
                // Green light period
                digitalWrite(greenLEDPin, HIGH);
                digitalWrite(redLEDPin, LOW);

                // Switch to red light after 2 seconds
                if (millis() - startTime >= 2000) {
                    currentLightState = true;
                    startTime = millis();
                    sensorStartTime = millis(); // Reset the sensor start time during the red phase
                }
            }
        } else {
            // Game over
            digitalWrite(greenLEDPin, LOW);
            digitalWrite(redLEDPin, LOW);

            // Check if the game ended due to motion or time expiration
            if (checkGameOutcome()) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Game Over!");
                lcd.setCursor(0, 1);
                lcd.print("Motion Detected");
                delay(2000);
            } else {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("You Win!");
                delay(2000);
            }

            gameStarted = false;
        }
    }
}

void checkMotionDuringRedLight() {
    if (currentLightState) {
        // Measure distance during the Red Light period
        float distance = readDistance();

        // Only consider significant changes in distance
        if (abs(distance - previousDistance) >= distanceThreshold) {
            previousDistance = distance;

            // Adjust the threshold based on your setup
            if (distance < 50) {
                // Motion detected, end the game
                digitalWrite(redLEDPin, LOW);
                for (int i = 0; i < 3; ++i) {
                    digitalWrite(redLEDPin, HIGH);
                    NewTone(buzzerPin, 2000, 200); // Buzzer warning during motion
                    delay(500);
                    digitalWrite(redLEDPin, LOW);
                    delay(500);
                }
                gameStarted = false;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Game Over!");
                lcd.setCursor(0, 1);
                lcd.print("Motion Detected");
                delay(2000);
            }
        }
    } else {
        // Reset the previous distance when not in the red light period
        previousDistance = 0.0;
    }
}

bool checkGameOutcome() {
    if (currentLightState) {
        // Measure distance during the Red Light period
        float distance = readDistance();

        // Only consider significant changes in distance
        if (abs(distance - previousDistance) >= distanceThreshold) {
            previousDistance = distance;

            // Adjust the threshold based on your setup
            if (distance < 50) {
                // Motion detected, end the game
                digitalWrite(redLEDPin, LOW);
                for (int i = 0; i < 3; ++i) {
                    digitalWrite(redLEDPin, HIGH);
                    NewTone(buzzerPin, 2000, 200); // Buzzer warning during motion
                    delay(500);
                    digitalWrite(redLEDPin, LOW);
                    delay(500);
                }
                return true; // Game over due to motion
            }
        }
    } else {
        // Reset the previous distance when not in the red light period
        previousDistance = 0.0;
    }

    return false; // Game still ongoing
}

void buttonInterrupt() {
    if (!gameStarted) {
        // Start or restart the game
        gameStarted = true;
        currentLightState = true; // Start with red light
        startTime = millis();
        remainingTime = 60;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Game Started!");

        // Start with green light for this version
        currentLightState = false;
        startTime = millis();
        sensorStartTime = millis(); // Reset the sensor start time
    } else {
        // Game is already running, pause the game
        gameStarted = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Game Paused");
    }
}

void updateRemainingTime() {
    if (gameStarted && remainingTime > 0) {
        remainingTime--;

        // Update the LCD with the remaining time
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Time Left: ");
        lcd.print(remainingTime);
    }
}
