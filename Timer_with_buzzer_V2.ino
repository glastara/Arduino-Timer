#include <LiquidCrystal.h>

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int buzzerPin = 9;      // Pin for the buzzer
const int buttonReset = 6;    // Pin for reset button
const int buttonSet = 7;      // Pin for timer set button
const int buttonStart = 13;    // Pin for start/pause/resume button - pin 8 unstable so using pin 13 instead, note that it toggles onboard LED

unsigned long timerDuration = 0; // Timer duration in milliseconds
unsigned long startTime = 0;     // Start time
unsigned long timePaused = 0;    // Time elapsed during pause
bool timerRunning = false;       // Timer state
bool timerPaused = false;        // Pause state
bool timerInitialized = false;   // Timer initialized flag
bool startButtonPressed = false; // Start button press flag to prevent immediate 'Time's Up!' & buzzer sound

void setup() {
  // Set up pins
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonSet, INPUT_PULLUP);
  pinMode(buttonStart, INPUT_PULLUP);
  pinMode(buttonReset, INPUT_PULLUP);

  // Initialize the LCD
  lcd.begin(16, 2); // Set LCD dimensions (16x2)
  lcd.print("Set Timer:"); // Display "Set Timer:"
  lcd.setCursor(0, 1); // Move to the second row
  lcd.print("0:00:00"); // Display "0:00:00" initially
}

void loop() {
  static int minutes = 0; // Store minutes

  // Set timer duration
  if (digitalRead(buttonSet) == LOW) {
    delay(200); // Debounce delay
    minutes = (minutes + 1) % 600; // Max 10 hours (600 minutes)
    lcd.setCursor(0, 1);
    lcd.print(minutes / 60); // Display hours
    lcd.print(":");
    if (minutes % 60 < 10) lcd.print("0"); // Add leading zero for minutes < 10
    lcd.print(minutes % 60);
    lcd.print(":00"); // Display seconds as 00
    timerInitialized = false; // Reset timer initialized flag on set
    startButtonPressed = false; // Reset start button press flag
  }

  // Start/Pause/Resume timer
  if (digitalRead(buttonStart) == LOW) {
    delay(200); // Debounce delay

    if (!startButtonPressed) {
      startButtonPressed = true; // Set the flag when start button is first pressed
      if (!timerRunning) {
        // Start the timer
        timerDuration = minutes * 60000; // Convert minutes to milliseconds
        startTime = millis();
        timerRunning = true;
        timerPaused = false;
        timePaused = 0;
        lcd.clear();
        lcd.print("Running...");
        timerInitialized = true; // Mark timer as initialized
      } else if (timerRunning && !timerPaused) {
        // Pause the timer
        timerPaused = true;
        timePaused = millis() - startTime; // Save elapsed time
        lcd.clear();
        lcd.print("Paused");
        // Display the current time when paused, and freeze it on the second row
        unsigned long pausedElapsed = millis() - startTime;
        unsigned long remaining = timerDuration - pausedElapsed;
        int hours = remaining / 3600000;
        int minutes = (remaining % 3600000) / 60000;
        int seconds = (remaining % 60000) / 1000;

        lcd.setCursor(0, 1);
        lcd.print(hours);
        lcd.print(":");
        if (minutes < 10) lcd.print("0");
        lcd.print(minutes);
        lcd.print(":");
        if (seconds < 10) lcd.print("0");
        lcd.print(seconds); // Keep this time on the second row while paused
      } else if (timerRunning && timerPaused) {
        // Resume the timer
        timerPaused = false;
        startTime = millis() - timePaused; // Adjust start time
        lcd.clear();
        lcd.print("Running...");
      }
    }
  }

  // Reset the timer if reset button is pressed
  if (digitalRead(buttonReset) == LOW) {
    delay(200); // Debounce delay
    timerRunning = false;  // Stop the timer
    timerPaused = false;   // Un-pause if it's paused
    timePaused = 0;        // Reset pause time
    lcd.clear();
    lcd.print("Set Timer:"); // Reset display to set time mode
    lcd.setCursor(0, 1);
    lcd.print("0:00:00"); // Display "0:00:00" initially
    timerInitialized = false; // Reset the initialized flag
    startButtonPressed = false; // Reset the start button press flag
  }

  // Timer countdown (only active when running and not paused)
  if (timerRunning && !timerPaused && timerInitialized) {
    unsigned long elapsed = millis() - startTime;
    unsigned long remaining = timerDuration - elapsed;

    // Calculate hours, minutes, and seconds remaining
    int hours = remaining / 3600000;
    int minutes = (remaining % 3600000) / 60000;
    int seconds = (remaining % 60000) / 1000;

    // Display the remaining time
    lcd.setCursor(0, 1);
    lcd.print(hours);
    lcd.print(":");
    if (minutes < 10) lcd.print("0");
    lcd.print(minutes);
    lcd.print(":");
    if (seconds < 10) lcd.print("0");
    lcd.print(seconds);

    // Check if the timer is complete
    if (elapsed >= timerDuration) {
      timerRunning = false;
      lcd.clear();
      lcd.print("Time's Up!");
      // Buzzer alert with increasing tone frequency
      for (int i = 0; i < 3; i++) {
        tone(buzzerPin, 1000 + (i * 200)); // Increase frequency for each buzz
        delay(200);
        noTone(buzzerPin);    // Stop tone
        delay(200);
      }
    }
  }
}
