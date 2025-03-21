#include <Arduino.h>

#define RED_LED 13
#define GREEN_LED 12
#define BLUE_LED A1
#define RED_BTN 8
#define YELLOW_BTN 2
#define GREEN_BTN 7
#define BLUE_BTN 6
#define RGB_BLUE 9
#define RGB_GREEN 10
#define RGB_RED 11
String inputString = "";
#define ROUND_POT A0
#define POT_BLUE A2
#define POT_GREEN A3
#define POT_RED A4

const unsigned long BLINK_INTERVAL = 2000;
unsigned long previousMillis = 0;
bool redButtonEnable = true;
bool blueButtonEnable = true;
bool greenButtonEnable = true;

bool ledState = false;
bool redToggle = false;
bool greenToggle = false;
bool blueToggle = false;
bool yellowToggle = false;
bool redBlink = true;
bool greenBlink = true;
bool blueBlink = true;
bool yellowBlink = true;
volatile bool resetFlag = false;
int blueBrightness = 1023;

void resetSystem() {
    resetFlag = true;
}

void setup() {
    blueBlink = true;
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(RED_BTN, INPUT_PULLUP);
    pinMode(GREEN_BTN, INPUT_PULLUP);
    pinMode(BLUE_BTN, INPUT_PULLUP);
    pinMode(YELLOW_BTN, INPUT_PULLUP);

    // RGB-konfig
    pinMode(RGB_BLUE, OUTPUT);
    pinMode(RGB_GREEN, OUTPUT);
    pinMode(RGB_RED, OUTPUT);
    pinMode(POT_BLUE, INPUT);
    pinMode(POT_GREEN, INPUT);
    pinMode(POT_RED, INPUT);
    
    attachInterrupt(digitalPinToInterrupt(YELLOW_BTN), resetSystem, FALLING);
    Serial.begin(115200);
    Serial.println("Ange kommando (L1 on/off, L2 on/off):");
}

void loop() {
    if (resetFlag) {
        redBlink = greenBlink = blueBlink = yellowBlink = true;
        redToggle = greenToggle = blueToggle = yellowToggle = false;
        redButtonEnable = true;
        greenButtonEnable = true;
        blueButtonEnable = true;
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        resetFlag = false;
    }

    if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Röd LED
    if (input.equalsIgnoreCase("L1 on")) {
        digitalWrite(RED_LED, HIGH);
        Serial.println("LED 1 är tänd!");
        redBlink = false;
    } else if (input.equalsIgnoreCase("L1 off")) {
        digitalWrite(RED_LED, LOW);
        Serial.println("LED 1 är släckt!");
        redBlink = false;
    } else if (input.equalsIgnoreCase("disable button 1")) {
        redButtonEnable = false;
        Serial.println("Knapp 1 (röd) är inaktiverad!");
    } else if (input.equalsIgnoreCase("enable button 1")) {
        redButtonEnable = true;
        Serial.println("Knapp 1 (röd) är aktiverad!");
    }

    // Grön LED
    else if (input.equalsIgnoreCase("L2 on")) {
        digitalWrite(GREEN_LED, HIGH);
        Serial.println("LED 2 är tänd!");
        greenBlink = false;
    } else if (input.equalsIgnoreCase("L2 off")) {
        digitalWrite(GREEN_LED, LOW);
        Serial.println("LED 2 är släckt!");
        greenBlink = false;
    } else if (input.equalsIgnoreCase("disable button 2")) {
        greenButtonEnable = false;
        Serial.println("Knapp 2 (grön) är inaktiverad!");
    } else if (input.equalsIgnoreCase("enable button 2")) {
        greenButtonEnable = true;
        Serial.println("Knapp 2 (grön) är aktiverad!");
    }

    // Blå LED
    else if (input.equalsIgnoreCase("L3 on")) {
        digitalWrite(BLUE_LED, HIGH);
        Serial.println("LED 3 är tänd!");
        blueBlink = false;
    } else if (input.equalsIgnoreCase("L3 off")) {
        digitalWrite(BLUE_LED, LOW);
        Serial.println("LED 3 är släckt!");
        blueBlink = false;
    } else if (input.equalsIgnoreCase("disable button 3")) {
        blueButtonEnable = false;
        Serial.println("Knapp 3 (blå) är inaktiverad!");
    } else if (input.equalsIgnoreCase("enable button 3")) {
        blueButtonEnable = true;
        Serial.println("Knapp 3 (blå) är aktiverad!");
    }
}


    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= BLINK_INTERVAL) {
        previousMillis = currentMillis;
        ledState = !ledState;
        
        if (redBlink) digitalWrite(RED_LED, ledState);
        if (greenBlink) digitalWrite(GREEN_LED, ledState);
        if (blueBlink) {
            blueBrightness = analogRead(ROUND_POT);
            blueBrightness = map(blueBrightness, 0, 1023, 0, 255);
            analogWrite(BLUE_LED, ledState ? blueBrightness : 0);
        }
    }
    if (redButtonEnable == true) {
    if (digitalRead(RED_BTN) == LOW) {
        delay(50);
        redBlink = false;
        if (digitalRead(RED_BTN) == LOW) {
            Serial.println("Red button pressed!");
            redToggle = !redToggle;
            digitalWrite(RED_LED, redToggle);
            while (digitalRead(RED_BTN) == LOW) delay(10);
        }
    }
}

if (greenButtonEnable == true) {  // Grön knappkontroll
    if (digitalRead(GREEN_BTN) == LOW) {
        delay(50);
        greenBlink = false;
        if (digitalRead(GREEN_BTN) == LOW) {
            Serial.println("Green button pressed!");
            greenToggle = !greenToggle;
            digitalWrite(GREEN_LED, greenToggle);
            while (digitalRead(GREEN_BTN) == LOW) delay(10);
        }
    }
}

if (blueButtonEnable == true) {  // Blå knappkontroll
    if (digitalRead(BLUE_BTN) == LOW) {
        delay(50);
        blueBlink = false;
        if (digitalRead(BLUE_BTN) == LOW) {
            Serial.println("Blue button pressed!");
            blueToggle = !blueToggle;
            digitalWrite(BLUE_LED, blueToggle);
            while (digitalRead(BLUE_BTN) == LOW) delay(10);
            Serial.print("BLUE_BTN: "); Serial.println(digitalRead(BLUE_BTN));
        }
    }
}


    int pot_blue_value = analogRead(POT_BLUE);
    analogWrite(RGB_BLUE, pot_blue_value);

    int pot_RED_value = analogRead(POT_RED);
    analogWrite(RGB_RED, pot_RED_value);
    
    int pot_GREEN_value = analogRead(POT_GREEN);
    analogWrite(RGB_GREEN, pot_GREEN_value);
}
