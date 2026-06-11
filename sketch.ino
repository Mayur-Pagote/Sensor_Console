#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
SCREEN_WIDTH,
SCREEN_HEIGHT,
&Wire,
-1
);

// Buttons
#define BTN_NAV D5
#define BTN_SELECT D6

// Sensors
#define TRIG D7
#define ECHO D8
#define ANALOG_PIN A0

enum State {
  MENU,
  DISTANCE,
  PULSE,
  MOISTURE
};

State currentState = MENU;

int menuIndex = 0;

const char* menuItems[] = {
  "Distance",
  "Heart Rate",
  "Soil Moisture"
};

unsigned long lastBeat = 0;
int bpm = 0;

void setup() {

  pinMode(BTN_NAV, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Wire.begin(D2, D1);

  display.begin(
  SSD1306_SWITCHCAPVCC,
  0x3C
  );

  display.clearDisplay();

  splash();
}

void loop() {

  switch(currentState) {

    case MENU:
      menuLoop();
      break;

    case DISTANCE:
      distanceLoop();
      break;

    case PULSE:
      pulseLoop();
      break;

    case MOISTURE:
      moistureLoop();
      break;
  }
}

void splash() {

  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(10,20);
  display.println("Sensor");
  display.println("Console");

  display.display();

  delay(2000);
}

bool navPressed() {

  if(!digitalRead(BTN_NAV)) {
    delay(200);
    return true;
  }
  return false;
}

bool selectPressed() {

  if(!digitalRead(BTN_SELECT)) {
    delay(200);
    return true;
  }
  return false;
}

void menuLoop() {

  if(navPressed()) {
    menuIndex++;
    if(menuIndex > 2) menuIndex = 0;
  }

  if(selectPressed()) {

    if(menuIndex == 0)
      currentState = DISTANCE;

    if(menuIndex == 1)
      currentState = PULSE;

    if(menuIndex == 2)
      currentState = MOISTURE;
  }

  display.clearDisplay();

  display.setTextSize(1);

  for(int i=0;i<3;i++) {

    if(i == menuIndex)
      display.print("> ");
    else
      display.print("  ");

    display.println(menuItems[i]);
  }

  display.display();
}

float getDistance() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration =
  pulseIn(ECHO,HIGH,30000);

  if(duration == 0)
    return -1;

  float dist =
  duration * 0.0343 / 2.0;

  return dist;
}

void distanceLoop() {

  if(selectPressed()) {
    currentState = MENU;
    return;
  }

  float distance = getDistance();

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,0);

  if(distance < 0) {

    display.println("Connect");
    display.println("HC-SR04 Sensor");

  } else {

    display.println("Distance");

    display.setTextSize(2);
    display.print(distance,1);
    display.println("cm");
  }

  display.display();

  delay(300);
}

void pulseLoop() {

  if(selectPressed()) {
    currentState = MENU;
    return;
  }

  int signal = analogRead(A0);

  static int threshold = 550;
  static bool beatDetected = false;

  if(signal > threshold && !beatDetected) {

    beatDetected = true;

    unsigned long now = millis();

    if(lastBeat > 0) {

      bpm = 60000 /
      (now - lastBeat);
    }

    lastBeat = now;
  }

  if(signal < threshold)
    beatDetected = false;

  display.clearDisplay();

  display.setTextSize(1);
  display.println("Heart Rate");

  if(signal < 50) {

    display.println("");
    display.println("Connect Pulse");
    display.println("Sensor");

  } else {

    display.setTextSize(2);
    display.print(bpm);
    display.println(" BPM");
  }

  display.display();

  delay(20);
}

void moistureLoop() {

  if(selectPressed()) {
    currentState = MENU;
    return;
  }

  int raw = analogRead(A0);

  int moisture =
  map(raw, 900, 300, 0, 100);

  moisture = constrain(
  moisture,
  0,
  100
  );

  display.clearDisplay();

  display.setTextSize(1);
  display.println("Soil Moisture");

  if(raw < 10) {

    display.println("");
    display.println("Connect");
    display.println("Sensor");

  } else {

    display.print("Raw:");
    display.println(raw);

    display.setTextSize(2);
    display.print(moisture);
    display.println("%");
  }

  display.display();

  delay(500);
}
