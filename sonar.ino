#include <math.h>     // include math lib

#include <Servo.h>        // servo control library
Servo myservo;  // create servo object to control a servo
int servPin = 2;        // create var 'servPin' for the servo pin

#include <Wire.h>       // libraries for controlling OLED
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
int screenWidth = 128;        // sets oled width 
int screenHight = 64;       // sets oled hight
int oledResetPin = -1;        // sets oled reset pin, shared with arduino reset so -1
Adafruit_SSD1306 display(screenWidth, screenHight, &Wire, oledResetPin);        // creates object to control display

#include <NewPing.h>
#define TRIGGER_PIN  7
#define ECHO_PIN     6
#define MAX_DISTANCE 30.48 // Maximum distance we want to measure (in centimeters). (1 foot)
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


int pos = 127;    // creates var 'pos' for servo position and write 127 to it

void setup() {
  Serial.begin(115200);       // begin serial int at 115200 baud
  
  myservo.attach(servPin);      // attatches servo object on pin 2
  myservo.write(pos);      // sets servo position to pos
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();        // init oled
  baseDisplay();
}

void loop() { 
  delay(500);
  baseDisplay();        // reset display
  float distanceLeft = calcDistance();    // get distance in inches
  plotPointLeft(distanceLeft);
  delay(500);

  for (pos = 127; pos > 82; pos -= 1) {       // move servo into pos 82 (middle) from pos 127 (left)
    myservo.write(pos);
    delay(15);
  }
  
  delay(500);
  baseDisplay();        // reset display
  float distanceCenter = calcDistance();    // get distance in inches (0 if more than 2 feet
  plotPointCenter(distanceCenter);
  delay(500);

  for (pos = 82; pos > 37; pos -= 1){       // move servo into pos 37 (right) from pos 82 (middle)
    myservo.write(pos);
    delay(15);
  }

  delay(500);
  baseDisplay();        // reset display
  float distanceRight = calcDistance();    // get distance in inches (0 if more than 2 feet)
  plotPointRight(distanceRight);
  delay(500);
  
  pos = 127;        // set pos var to 127 (left)
  myservo.write(pos);       // move servo to position 127 (left) from 37 (right)
}

void baseDisplay(void) {
  display.clearDisplay();       // set display blank
  display.display();        // write to display
  display.fillCircle(64, 0, 2, WHITE);        // draw small filled circle in the center with a diameter of 2
  display.drawCircle(64, 0, 10, WHITE);       // draw smallest inside circle with a diameter of 10
  display.drawCircle(64, 0, 35, WHITE);       // draw middle circle with a diameter of 35
  display.drawCircle(64, 0, 60, WHITE);       // draw largest outside circle with diameter of 60
  display.display();
}

int calcDistance() {
  delay(50);
  float distance = sonar.ping_cm();       // get distance in cm
  float distanceIn = distance / 2.54;       // convert distance to in
  return distanceIn;
}

void plotPointRight(float distance) {
  float radius = distance * 5;        //find how far the point is from center based on distance
  float rawX = (radius * (sin(5.49779))) + 64;
  float rawY = radius * (cos(5.49779));
  int x = round(rawX);
  int y = round(rawY);
  display.fillCircle(x, y, 2, WHITE);
  display.display();
}

void plotPointCenter(float distance) {
  int x = 64;        // set x to 64 (center display)
  float rawY = distance * 5;        // convert distance in inches to a pixel on display
  int y = round(rawY);        // round float y to the nearest integer
  display.fillCircle(x, y, 2, WHITE);       // plot point
  display.display();        // write to display
}

void plotPointLeft(float distance) {
  float radius = distance * 5;        //find how far the point is from center based on distance
  float rawX = (radius * (sin(0.785398))) + 64;
  float rawY = radius * (cos(0.785398));
  int x = round(rawX);
  int y = round(rawY);
  display.fillCircle(x, y, 2, WHITE);
  display.display();
}