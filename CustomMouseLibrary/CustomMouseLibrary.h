#ifndef CustomMouseLibrary_h
#define CustomMouseLibrary_h

#include "HID.h"

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else

#define CustomMouseLibrary_LEFT 1
#define CustomMouseLibrary_RIGHT 2
#define CustomMouseLibrary_MIDDLE 4
#define CustomMouseLibrary_ALL (CustomMouseLibrary_LEFT | CustomMouseLibrary_RIGHT | CustomMouseLibrary_MIDDLE)

class CustomMouseLibrary_
{
private:
  uint8_t _buttons;
  void buttons(uint8_t b);

public:
  CustomMouseLibrary_(void);
  void begin(void);
  void end(void);
  void click(uint8_t b = CustomMouseLibrary_LEFT);
  void move(signed char x, signed char y, signed char h = 0, signed char v = 0);
  void press(uint8_t b = CustomMouseLibrary_LEFT);     // press LEFT by default
  void release(uint8_t b = CustomMouseLibrary_LEFT);   // release LEFT by default
  bool isPressed(uint8_t b = CustomMouseLibrary_LEFT); // check LEFT by default
};
extern CustomMouseLibrary_ CustomMouseLibrary;

#endif
#endif
