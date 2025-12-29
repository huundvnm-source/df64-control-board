#include <Arduino.h>
#include <TaskScheduler.h>
#include <OneButton.h>

// --- Pin Definitions ---
// D5 on NodeMCU is GPIO 14
// D6 on NodeMCU is GPIO 12
const int PIN_BTN   = 14; 
const int PIN_RELAY = 12;

// --- Constants ---
const unsigned long AUTO_OFF_MS = 60000; // 60 seconds

// --- Global Objects ---
// Button on PIN 14, Active LOW (press connects to GND), Enable Pullup
OneButton btn(PIN_BTN, true, true); 
Scheduler runner;

// --- State Variables ---
bool relayState = false;     // Current hardware state
bool isLatched = false;      // logic flag: true if turned on by CLICK
unsigned long latchStartTime = 0; 

// --- Helper Functions ---
void setRelay(bool state) {
    relayState = state;
    digitalWrite(PIN_RELAY, state ? HIGH : LOW);
    Serial.printf("Relay: %s\n", state ? "ON" : "OFF");
}

// --- Button Callbacks ---

// 1. Single Click: Toggle Logic
void handleClick() {
    if (relayState) {
        // If ON, turn OFF (regardless of how it turned on)
        setRelay(false);
        isLatched = false;
    } else {
        // If OFF, turn ON and start timer
        setRelay(true);
        isLatched = true;
        latchStartTime = millis();
        Serial.println("Action: Latch ON (Auto-off in 60s)");
    }
}

// 2. Long Press Start: Momentary ON
void handleLongPressStart() {
    Serial.println("Action: Hold Active, Relay ON");
    // Force ON immediately
    setRelay(true);
    // Mark as NOT latched so timer doesn't interfere weirdly
    isLatched = false; 
    
}

// 3. Long Press Stop: Momentary OFF
void handleLongPressStop() {
    Serial.println("Action: Hold Released, Relay OFF");  
    setRelay(false); 
}

// --- Tasks ---

// Task 1: Check Button (Runs every 5ms for responsiveness)
void buttonTaskCallback() {
    btn.tick();
}
Task tButton(5, TASK_FOREVER, &buttonTaskCallback);

// Task 2: Auto-Stop Timer (Runs every 1000ms)
void timeoutTaskCallback() {
    if (relayState && isLatched) {
        // Only check timeout if we are in "Latched" mode
        if (millis() - latchStartTime >= AUTO_OFF_MS) {
            Serial.println("Timer: 60s Limit Reached. Turning OFF.");
            setRelay(false);
            isLatched = false;
        }
    }
}
Task tTimeout(1000, TASK_FOREVER, &timeoutTaskCallback);

// --- Main Setup ---
void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("\n--- ESP8266 Relay Controller ---");

    // Hardware Setup
    pinMode(PIN_RELAY, OUTPUT);
    setRelay(false); // Ensure OFF at boot

    // Button Setup
    // Link the library events to our functions
    btn.attachClick(handleClick);
    btn.attachLongPressStart(handleLongPressStart);
    btn.attachLongPressStop(handleLongPressStop);
    btn.setPressMs(500); // 500ms hold is considered a "Long Press"

    // Scheduler Setup
    runner.init();
    runner.addTask(tButton);
    runner.addTask(tTimeout);
    
    tButton.enable();
    tTimeout.enable();
}

void loop() {
    runner.execute();
}
