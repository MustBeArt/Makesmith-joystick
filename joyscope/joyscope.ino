/*
 * Joystick control of MakesmithCNC
 * 
 * Connect an analog joystick to pins A13 (x) and A14 (y), and a pushbutton
 * (preferably integrated with the joystick) on pin A15.
 * 
 * The pushbutton switches between X-Y mode, in which the joystick moves the
 * head around horizontally, and Z mode, in which the joystick moves the head
 * up and down.
 * 
 * Copyright 2017 Paul Williamson, paul@mustbeart.com
 * 
 */

#include <Servo.h>
#include <Bounce2.h>

#define XSERVO 5
#define YSERVO 6
#define ZSERVO 7

#define SERVO_STOPPED 90

#define JOYX    A13
#define JOYY    A14
#define JOYSW   A15

#define JOYMODE_NONE  0
#define JOYMODE_XY    1
#define JOYMODE_Z     2

Servo x, y, z;
Bounce joy_button;

int joystick_mode = JOYMODE_NONE; // Button must be clicked before using joystick (for now)

int xzero, yzero;   // Joystick readings at startup, assumed to be neutral position

int xdead = 10;     // number of A/D counts around neutral to ignore
int ydead = 10;
int xscale = 10;    // number of A/D counts per unit of servo output value
int yscale = 10;
int zscale = 10;

void setup(){
	Serial.begin(19200);

  // initialize all servos to stopped
	x.write(SERVO_STOPPED); y.write(SERVO_STOPPED); z.write(SERVO_STOPPED);
	Serial.println("ready");

	analogReference(EXTERNAL);

  pinMode(JOYX, INPUT);
  pinMode(JOYY, INPUT);
  pinMode(JOYSW, INPUT_PULLUP);

  delay(100);   // settling time, just in case

  // Record the initial (assumed neutral) position of the joystick.
  xzero = analogRead(JOYX);
  yzero = analogRead(JOYY);

  // Set up the button debouncer
  joy_button.interval(50);
  joy_button.attach(JOYSW);

}


void loop(){

  int xjoy, yjoy;

  // Test mode: use keys on serial terminal to control servos
  
  if (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case 'q': Serial.println("left");
                x.attach(XSERVO);
                x.write(x.read()+1);
                break;

      case 'a': Serial.println("stopx");
                x.write(SERVO_STOPPED);
                x.detach();
                break;

      case 'z': Serial.println("right");
                x.attach(XSERVO);
                x.write(x.read()-1);       //should wrap, but this is just a test
                break;
                
      case 'w': Serial.println("back");
                y.attach(YSERVO);
                y.write(0);
                break;

      case 's': Serial.println("stopy");
                y.write(SERVO_STOPPED);
                y.detach();
                break;

      case 'x': Serial.println("front");
                y.attach(YSERVO);
                y.write(180);
                break;

      case 'e': Serial.println("up");
                z.attach(ZSERVO);
                z.write(180);
                break;

      case 'd': Serial.println("stopz");
                z.write(SERVO_STOPPED);
                z.detach();
                break;

      case 'c': Serial.println("down");
                z.attach(ZSERVO);
                z.write(0);
                break;
                
      case 'j': Serial.print("joystick: ");
                Serial.print(digitalRead(JOYX));
                Serial.print(" ");
                Serial.print(digitalRead(JOYY));
                if (digitalRead(JOYSW))
                  Serial.println(" +");
                else
                  Serial.println("");
                break;

      default:  Serial.println("unknown key");
                break;
    }
  }

  // Read the joystick, adjust value to center around zero.
  xjoy = analogRead(JOYX) - xzero;
  yjoy = analogRead(JOYY) - yzero;
  
  if (joystick_mode == JOYMODE_XY) {
    if (abs(xjoy) < xdead) {
       x.write(SERVO_STOPPED);
       x.detach();
    } else {
       x.attach(XSERVO);
       x.write(SERVO_STOPPED + xjoy / xscale);
    }

    if (abs(yjoy) < ydead) {
      y.write(SERVO_STOPPED);
      y.detach();
    } else {
      y.attach(YSERVO);
      y.write(SERVO_STOPPED + yjoy / yscale);
    }
  } else if (joystick_mode == JOYMODE_Z) {
    if (abs(yjoy) < ydead) {
      z.write(SERVO_STOPPED);
      z.detach();
    } else {
      z.attach(ZSERVO);
      z.write(SERVO_STOPPED + yjoy / zscale);
    }
  }


  // Handle the button. First press initiates XY mode (from NONE mode).
  // Each subsequent press toggles between XY mode and Z mode.
  joy_button.update();
  if (joy_button.fell()) {
    if (joystick_mode == JOYMODE_XY) {
      joystick_mode = JOYMODE_Z;
      Serial.println("Focus mode");
    }
    else {
      joystick_mode = JOYMODE_XY;
      Serial.println("XY mode");
    }
  }
  
}
