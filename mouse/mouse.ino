#include <Bounce2.h>

#include <Mouse.h>
#include <Keyboard.h>

const int pingRockerY = A0;
const int pingRockerX = A1;

const int pingButtonBlue = 2;
const int pingButtonGreen = 3;
const int pingButtonRed = 4;

const int pingWheelLeft = 5;
const int pingWheelRight = 6;

int responseDelay = 5;

//摇杆 Y轴:负数是前 正数是后 X轴:负数是右 正数是左
int axisY;
int axisX;
int lastAxisY;
int lastAxisX;
int tempAxis;
int distance;

//按钮
boolean leftButtonStatus = false;
boolean middleButtonStatus = false;
boolean rightButtonStatus = false;

//滚轮
int tempLeftWheel;
int tempRightWheel;
int lastLeftWheel;
int lastRightWheel;

Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();


void setup() {
  pinMode(pingButtonBlue, INPUT);
  pinMode(pingButtonGreen, INPUT);
  pinMode(pingButtonRed, INPUT);
  pinMode(pingRockerY, INPUT);
  pinMode(pingRockerX, INPUT);
  pinMode(pingWheelRight, INPUT);
  pinMode(pingWheelLeft, INPUT);
  //保证复位
  while ( lastAxisY < 510 | lastAxisY > 538 | lastAxisX < 510 | lastAxisX > 538) {
    lastAxisY = analogRead(pingRockerX);
    lastAxisX = analogRead(pingRockerY);
  }
  debouncer1.attach(pingWheelRight);
  debouncer2.attach(pingWheelLeft);
  debouncer1.interval(20);
  debouncer2.interval(20);
  debouncer1.update();
  debouncer2.update();
  lastRightWheel = debouncer1.read();
  lastLeftWheel = debouncer2.read();
  Mouse.begin();
  Keyboard.begin();
}

void loop() {
  moveMouse();
  clickMouse();
  rollWheel();
  delay(responseDelay);
}

void rollWheel() {
  debouncer1.update();
  debouncer2.update();
  tempRightWheel = debouncer1.read();
  tempLeftWheel = debouncer2.read();
  if ( tempLeftWheel == lastLeftWheel & tempRightWheel != lastRightWheel) {
    Keyboard.press(KEY_PAGE_UP);
    delay(5);
    Keyboard.releaseAll();
  }
  if ( tempRightWheel == lastRightWheel & tempLeftWheel != lastLeftWheel ) {
    Keyboard.press(KEY_PAGE_DOWN);
    delay(5);
    Keyboard.releaseAll();
  }
  lastRightWheel = tempRightWheel;
  lastLeftWheel = tempLeftWheel;
  Serial.println(tempRightWheel + "-" + tempLeftWheel);
}


void clickMouse() {
  if (digitalRead(pingButtonBlue) == LOW) {
    if (leftButtonStatus == false) {
      Mouse.press(MOUSE_LEFT);
      leftButtonStatus = true;
    }
  } else {
    leftButtonStatus = false;
    if (Mouse.isPressed() == true) {
      Mouse.release(MOUSE_LEFT);
    }
  }

  if (digitalRead(pingButtonGreen) == LOW) {
    if (middleButtonStatus == false) {
      Mouse.click(MOUSE_MIDDLE);
      middleButtonStatus = true;
    }
  } else {
    middleButtonStatus = false;
  }

  if (digitalRead(pingButtonRed) == LOW) {
    if (rightButtonStatus == false) {
      Mouse.click(MOUSE_RIGHT);
      rightButtonStatus = true;
    }
  } else {
    rightButtonStatus = false;
  }
}

void moveMouse() {
  axisY = readAxis(pingRockerX, lastAxisY);
  axisX = readAxis(pingRockerY, lastAxisX);
  if ((axisY != 0) || (axisX != 0)) {
    Mouse.move(-axisY, axisX, 0);
  }
}

int readAxis(int thisAxis, int lastAxis) {
  tempAxis = analogRead(thisAxis);
  distance = tempAxis - lastAxis;
  if (distance > -2 & distance < 2) {
    distance = 0;
  }
  lastAxis = tempAxis;
  return distance / 80 ;
}
