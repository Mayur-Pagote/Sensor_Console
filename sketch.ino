#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================= PIN DEFINITIONS =================
#define BTN_NAV     D5
#define BTN_SELECT  D6

#define TRIG_PIN    D7
#define ECHO_PIN    D8

#define ANALOG_PIN  A0

// ================= MENU STATES =================
enum Mode {
  MENU,
  DISTANCE,
  HEART,
  SOIL
};

Mode currentMode = MENU;

// ================= MENU ITEMS =================
const char* menuItems[] = {
  "Distance",
  "Heart Rate",
  "Soil Moisture"
};

int menuIndex = 0;

// ================= BUTTON VARIABLES =================
bool lastNavState = HIGH;
bool lastSelectState = HIGH;

unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 150;

// ================= HEART VARIABLES =================
int bpm = 0;
unsigned long lastBeat = 0;
bool pulseDetected = false;

// ====================================================

void setup() {

  Serial.begin(115200);
  delay(1000);

  Wire.begin(D2, D1);

  pinMode(BTN_NAV, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    Serial.println("OLED NOT FOUND");

    while (true) {
      delay(100);
    }
  }

  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();

  splashScreen();
}

void loop() {

  handleButtons();

  switch (currentMode) {

    case MENU:
      drawMenu();
      break;

    case DISTANCE:
      distanceScreen();
      break;

    case HEART:
      heartScreen();
      break;

    case SOIL:
      soilScreen();
      break;
  }
}

// ====================================================

void splashScreen() {

  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(10, 15);
  display.println("Sensor");

  display.setCursor(10, 40);
  display.println("Console");

  display.display();

  delay(2000);
}

// ====================================================

void handleButtons() {

  bool nav = digitalRead(BTN_NAV);
  bool sel = digitalRead(BTN_SELECT);

  if (millis() - lastDebounce > debounceDelay) {

    if (nav == LOW && lastNavState == HIGH) {

      if (currentMode == MENU) {

        menuIndex++;

        if (menuIndex > 2)
          menuIndex = 0;
      }

      lastDebounce = millis();
    }

    if (sel == LOW && lastSelectState == HIGH) {

      if (currentMode == MENU) {

        if (menuIndex == 0) currentMode = DISTANCE;
        if (menuIndex == 1) currentMode = HEART;
        if (menuIndex == 2) currentMode = SOIL;
      }
      else {

        currentMode = MENU;
      }

      lastDebounce = millis();
    }
  }

  lastNavState = nav;
  lastSelectState = sel;
}

// ====================================================

void drawMenu() {

  static unsigned long lastRefresh = 0;

  if (millis() - lastRefresh < 100)
    return;

  lastRefresh = millis();

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("Sensor Console");

  display.drawLine(0, 10, 128, 10, WHITE);

  for (int i = 0; i < 3; i++) {

    display.setCursor(0, 18 + i * 15);

    if (i == menuIndex)
      display.print("> ");
    else
      display.print("  ");

    display.println(menuItems[i]);
  }

  display.display();
}

// ====================================================

float getDistance() {

  float total = 0;
  int validReadings = 0;

  for (int i = 0; i < 5; i++) {

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duration > 0) {

      float distance = duration * 0.0343 / 2.0;

      total += distance;
      validReadings++;
    }

    delay(10);
  }

  if (validReadings == 0)
    return -1;

  return total / validReadings;
}

// ====================================================

void distanceScreen() {

  float distance = getDistance();

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("Distance Sensor");

  display.drawLine(0, 10, 128, 10, WHITE);

  if (distance < 0) {

    display.setCursor(0, 25);
    display.println("Connect HC-SR04");
  }
  else {

    display.setTextSize(2);

    display.setCursor(10, 30);
    display.print(distance, 1);
    display.print("cm");
  }

  display.display();

  delay(250);
}

// ====================================================

void soilScreen() {

  int raw = analogRead(A0);

  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("Soil Moisture");

  display.drawLine(0, 10, 128, 10, WHITE);

  if (raw < 20) {

    display.setCursor(0, 25);
    display.println("Connect Sensor");
  }
  else {

    int moisture = map(raw, 900, 300, 0, 100);

    moisture = constrain(moisture, 0, 100);

    display.setCursor(0, 22);
    display.print("ADC: ");
    display.println(raw);

    display.setCursor(0, 42);
    display.print("Moisture: ");
    display.print(moisture);
    display.print("%");
  }

  display.display();

  delay(300);
}

// ====================================================

void heartScreen() {

  int signal = analogRead(A0);

  const int threshold = 550;

  if (signal > threshold && !pulseDetected) {

    pulseDetected = true;

    unsigned long now = millis();

    if (lastBeat > 0) {

      bpm = 60000 / (now - lastBeat);

      if (bpm < 40 || bpm > 180)
        bpm = 0;
    }

    lastBeat = now;
  }

  if (signal < threshold)
    pulseDetected = false;

  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("Heart Rate");

  display.drawLine(0, 10, 128, 10, WHITE);

  if (signal < 50) {

    display.setCursor(0, 25);
    display.println("Connect Sensor");
  }
  else {

    display.setTextSize(2);

    display.setCursor(10, 25);
    display.print(bpm);
    display.print(" BPM");

    if (pulseDetected) {

      display.fillCircle(115, 50, 5, WHITE);
    }
  }

  display.display();

  delay(20);
}
