#include <Mouse.h>
#include <Keyboard.h>

const int pingRockerY = A0;
const int pingRockerX = A1;

const int pingButtonBlue = 15;
//const int pingButtonGreen = 14;
const int pingButtonRed = 16;

const int pingWheelLeft = 2;
const int pingWheelRight = 3;

int responseDelay = 5;

//摇杆 Y轴:负数是前 正数是后 X轴:负数是右 正数是左
int axisY;
int axisX;
int lastAxisY;
int lastAxisX;
int tempAxis;
int distance;
int distanceParam = 80;

//按钮
boolean leftButtonStatus = false;
boolean middleButtonStatus = false;
boolean rightButtonStatus = false;

//滚轮
boolean A_set;
boolean B_set;

void setup() {
  pinMode(pingButtonBlue, INPUT);
//  pinMode(pingButtonGreen, INPUT);
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
    A_set = true;
    if (!B_set) {
      Keyboard.press(KEY_PAGE_UP);
      delay(1);
      Keyboard.releaseAll();
    }
  }
  if (digitalRead(pingWheelLeft) == LOW) {
    A_set = false;
  }
}

void wheelTurnDown() {
  if (digitalRead(pingWheelRight) == HIGH) {
    B_set = true;
    if (!A_set) {
      Keyboard.press(KEY_PAGE_DOWN);
      delay(1);
      Keyboard.releaseAll();
    }
  }
  if (digitalRead(pingWheelRight) == LOW) {
    B_set = false;
  }
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

//  if (digitalRead(pingButtonGreen) == LOW) {
//    if (middleButtonStatus == false) {
//      Mouse.click(MOUSE_MIDDLE);
//      middleButtonStatus = true;
//    }
//  } else {
//    middleButtonStatus = false;
//  }

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
  return distance / distanceParam ;
}
