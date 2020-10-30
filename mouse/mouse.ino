#include <Mouse.h>
#include <Keyboard.h>

const int pingRockerX = A0;
const int pingRockerY = A1;

const int pingButtonLeft = 15;
//const int pingButtonMiddle = 14;
const int pingButtonRight = 16;

const int pingWheelLeft = 2;
const int pingWheelRight = 3;

int responseDelay = 5;

//摇杆 Y轴:负数是前 正数是后 X轴:负数是右 正数是左
int axisX;
int axisY;
int lastAxisX;
int lastAxisY;
int tempAxis;
int distance;
int distanceParam = 80;

//按钮
boolean leftButtonStatus = false;
boolean middleButtonStatus = false;
boolean rightButtonStatus = false;

//滚轮
boolean wheelFlagA;
boolean wheelFlagB;

void setup() {
  pinMode(pingButtonLeft, INPUT);
//  pinMode(pingButtonMiddle, INPUT);
  pinMode(pingButtonRight, INPUT);
  pinMode(pingRockerX, INPUT);
  pinMode(pingRockerY, INPUT);
  pinMode(pingWheelRight, INPUT);
  pinMode(pingWheelLeft, INPUT);
  //保证复位
  while ( lastAxisY < 510 | lastAxisY > 538 | lastAxisX < 510 | lastAxisX > 538) {
    lastAxisY = analogRead(pingRockerY);
    lastAxisX = analogRead(pingRockerX);
  }
  attachInterrupt(0, wheelTurnUp, CHANGE);
  attachInterrupt(1, wheelTurnDown, CHANGE);
  Mouse.begin();
  Keyboard.begin();
  Serial.begin (9600);
}

void loop() {
  moveMouse();
  clickMouse();
  delay(responseDelay);
}

void wheelTurnUp() {
  if (digitalRead(pingWheelLeft) == HIGH) {
    wheelFlagA = true;
    if (!wheelFlagB) {
      Keyboard.press(KEY_PAGE_UP);
      delay(1);
      Keyboard.releaseAll();
    }
  }
  if (digitalRead(pingWheelLeft) == LOW) {
    wheelFlagA = false;
  }
}

void wheelTurnDown() {
  if (digitalRead(pingWheelRight) == HIGH) {
    wheelFlagB = true;
    if (!wheelFlagA) {
      Keyboard.press(KEY_PAGE_DOWN);
      delay(1);
      Keyboard.releaseAll();
    }
  }
  if (digitalRead(pingWheelRight) == LOW) {
    wheelFlagB = false;
  }
}

void clickMouse() {
  if (digitalRead(pingButtonLeft) == LOW) {
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

//  if (digitalRead(pingButtonMiddle) == LOW) {
//    if (middleButtonStatus == false) {
//      Mouse.click(MOUSE_MIDDLE);
//      middleButtonStatus = true;
//    }
//  } else {
//    middleButtonStatus = false;
//  }

  if (digitalRead(pingButtonRight) == LOW) {
    if (rightButtonStatus == false) {
      Mouse.click(MOUSE_RIGHT);
      rightButtonStatus = true;
    }
  } else {
    rightButtonStatus = false;
  }
}

void moveMouse() {
  axisY = readAxis(pingRockerY, lastAxisY);
  axisX = readAxis(pingRockerX, lastAxisX);
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
  return distance / distanceParam ;
}
