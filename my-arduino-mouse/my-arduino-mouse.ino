#include <Keyboard.h>
#include <CustomMouseLibrary.h>

//针脚定义
const int pingRockerX = A0;
const int pingRockerY = A1;
const int pingRockerButton = 16;

const int pingButtonLeft = 4;
const int pingButtonMiddle = 5;
const int pingButtonRight = 6;
const int pingWheelS1 = 3;
const int pingWheelS2 = 2;
const int pingWheelButton = 10;

//常量 每轮循环间隔 灵敏度(越小越灵敏)
const int responseDelay = 5;
const int distanceParam = 80;

//读数临时变量 摇杆 Y轴:负数是前 正数是后 X轴:负数是右 正数是左
int lastAxisX;
int lastAxisY;
bool wheelFlagA;
bool wheelFlagB;

// CustomMouseLibrary.move(0, 0, 顺上逆下, 顺右逆左); 后两个为窗口方向
void setup()
{
  pinMode(pingRockerX, INPUT);
  pinMode(pingRockerY, INPUT);
  pinMode(pingRockerButton, INPUT);
  pinMode(pingButtonLeft, INPUT);
  pinMode(pingButtonMiddle, INPUT);
  pinMode(pingButtonRight, INPUT);
  pinMode(pingWheelS2, INPUT);
  pinMode(pingWheelS1, INPUT);
  pinMode(pingWheelButton, INPUT);
  //保证摇杆位于初始位置 当偏离值(标准应该在524左右)不大 并且多次读数均一致表示摇杆位于初始静止
  int temp = 0;
  while (lastAxisY < 510 || lastAxisY > 538 || lastAxisX < 510 || lastAxisX > 538)
  {
    lastAxisY = analogRead(pingRockerY);
    lastAxisX = analogRead(pingRockerX);
    if (temp > 10)
    {
      break;
    }
    else
    {
      temp++;
    }
    delay(responseDelay);
  }
  //监听滚轮中断
  attachInterrupt(digitalPinToInterrupt(pingWheelS1), wheelClockwise, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pingWheelS2), wheelAnticlockwise, CHANGE);
  CustomMouseLibrary.begin();
  Keyboard.begin();
}

void loop()
{
  listenMouseMove();
  listenClickButton();
  delay(responseDelay);
}

void sendWheel(int num)
{
  if (digitalRead(pingWheelButton) == HIGH)
  {
    CustomMouseLibrary.move(0, 0, num, 0);
  }
  else
  {
    CustomMouseLibrary.move(0, 0, 0, num);
  }
}

//逆时针
void wheelAnticlockwise()
{
  if (digitalRead(pingWheelS1) == HIGH)
  {
    wheelFlagB = true;
    if (!wheelFlagA)
    {
      sendWheel(-1);
    }
  }
  else
  {
    wheelFlagB = false;
  }
}

//顺时针
void wheelClockwise()
{
  if (digitalRead(pingWheelS2) == HIGH)
  {
    wheelFlagA = true;
    if (!wheelFlagB)
    {
      sendWheel(1);
    }
  }
  else
  {
    wheelFlagA = false;
  }
}

void sendButton(int Buttonpin, uint8_t Button)
{
  if (digitalRead(Buttonpin) == LOW)
  {
    if (!CustomMouseLibrary.isPressed(Button))
    {
      CustomMouseLibrary.press(Button);
    }
  }
  else if (CustomMouseLibrary.isPressed(Button))
  {
    CustomMouseLibrary.release(Button);
  }
}

void listenClickButton()
{
  sendButton(pingButtonLeft, CustomMouseLibrary_LEFT);
  sendButton(pingButtonMiddle, CustomMouseLibrary_MIDDLE);
  sendButton(pingButtonRight, CustomMouseLibrary_RIGHT);
}

void listenMouseMove()
{
  int axisY = readAxis(pingRockerY, lastAxisY);
  int axisX = readAxis(pingRockerX, lastAxisX);
  if (axisY != 0 || axisX != 0)
  {
    CustomMouseLibrary.move(-axisY, axisX, 0, 0);
  }
}

int readAxis(int pin, int lastAxis)
{
  int nowAxis = analogRead(pin);
  int distance = nowAxis - lastAxis;
    lastAxis = nowAxis;
  if (distance > -2 & distance < 2)
  {
    return 0;
  }
  return distance / distanceParam;
}
