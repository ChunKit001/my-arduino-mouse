#include <Keyboard.h>
#include <CustomMouseLibrary.h>


const int pingRockerX = A0;
const int pingRockerY = A1;

const int pingButtonLeft = 15;
const int pingButtonMiddle = 14;
const int pingButtonRight = 16;

const int pingWheelLeft = 2;
const int pingWheelRight = 3;
const int pingWheelButton = 10;

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
  pinMode(pingButtonMiddle, INPUT);
  pinMode(pingButtonRight, INPUT);
  pinMode(pingRockerX, INPUT);
  pinMode(pingRockerY, INPUT);
  pinMode(pingWheelRight, INPUT);
  pinMode(pingWheelLeft, INPUT);
  pinMode(pingWheelButton, INPUT);
  //保证复位
  while ( lastAxisY < 510 | lastAxisY > 538 | lastAxisX < 510 | lastAxisX > 538) {
    lastAxisY = analogRead(pingRockerY);
    lastAxisX = analogRead(pingRockerX);
  }
  attachInterrupt(0, wheelTurnUp, CHANGE);
  attachInterrupt(1, wheelTurnDown, CHANGE);
  CustomMouseLibrary.begin();
  Keyboard.begin();
  Serial.begin (9600);
}

void loop() {
  moveMouse();
  clickMouse();
  delay(responseDelay);
}

void wheelTurnUp() {
  if (digitalRead(pingWheelButton) == LOW) {
    if (digitalRead(pingWheelLeft) == HIGH) {
      wheelFlagA = true;
      if (!wheelFlagB) {
        CustomMouseLibrary.move(0, 0, 1, 0);
      }
    }
    if (digitalRead(pingWheelLeft) == LOW) {
      wheelFlagA = false;
    }
  } else {
    if (digitalRead(pingWheelLeft) == HIGH) {
      wheelFlagA = true;
      if (!wheelFlagB) {
        CustomMouseLibrary.move(0, 0, 0, 1);
      }
    }
    if (digitalRead(pingWheelLeft) == LOW) {
      wheelFlagA = false;
    }
  }
}

void wheelTurnDown() {
  if (digitalRead(pingWheelButton) == LOW) {
    if (digitalRead(pingWheelRight) == HIGH) {
      wheelFlagB = true;
      if (!wheelFlagA) {
        CustomMouseLibrary.move(0, 0, -1, 0);
      }
    }
    if (digitalRead(pingWheelRight) == LOW) {
      wheelFlagB = false;
    }
  } else {
    if (digitalRead(pingWheelRight) == HIGH) {
      wheelFlagB = true;
      if (!wheelFlagA) {
        CustomMouseLibrary.move(0, 0, 0, -1);
      }
    }
    if (digitalRead(pingWheelRight) == LOW) {
      wheelFlagB = false;
    }
  }
}

void clickMouse() {
  if (digitalRead(pingButtonLeft) == LOW) {
    if (leftButtonStatus == false) {
      CustomMouseLibrary.press(CustomMouseLibrary_LEFT);
      leftButtonStatus = true;
    }
  } else {
    leftButtonStatus = false;
    if (CustomMouseLibrary.isPressed() == true) {
      CustomMouseLibrary.release(CustomMouseLibrary_LEFT);
    }
  }

  if (digitalRead(pingButtonMiddle) == LOW) {
    if (middleButtonStatus == false) {
      CustomMouseLibrary.press(CustomMouseLibrary_MIDDLE);
      middleButtonStatus = true;
    }
  } else {
    middleButtonStatus = false;
  }

  if (digitalRead(pingButtonRight) == LOW) {
    if (rightButtonStatus == false) {
      CustomMouseLibrary.click(CustomMouseLibrary_RIGHT);
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
    CustomMouseLibrary.move(-axisY, axisX, 0, 0);
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
