/*
* Copyright (c) 2010  Chris Lockfort
*
* Permission to use, copy, modify, and distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef roomba_include
#define roomba_include

#include "WConstants.h"
#undef int
#undef float
#undef abs
#undef round

class Roomba{
 public:
  Roomba();
  void goForward();
  void goBackward();
  void goLeft();
  void goRight();
  void drive(int velocity, int radius);
  void disco();
  void led(char type);
  void writeLEDs();
  void rawWriteLEDs();
  void updateSensors();
  void setBaud(int baudRate);
  void sleep();
  void wake();
  void spotClean();
  void clean();
  void maxClean();
  void PCJrPlay(char song[]);
  void PCJrTempo(int tempo);
  void setSong(char song[], char songLength, char songNumber);
  void playSong(char songNumber);


  //Sensor values (Public so user can directly take/set software interrupts)
  bool bumpLeft;
  bool bumpRight;
  bool wheeldropLeft;
  bool wheeldropRight;
  bool wall;
  bool cliffLeft;
  bool cliffFrontLeft;
  bool cliffFrontRight;
  bool cliffRight;
  bool virtualWall;
  bool motorDriveLeftOvercurrent;
  bool motorDriveRightOvercurrent;
  bool motorBrushOvercurrent;
  bool motorVacuumOvercurrent;
  bool motorSidebrushOvercurrent;
  unsigned char dirtDetectLeft;
  unsigned char dirtDetectRight;
  unsigned char RCcommand;
  bool powerButton;
  bool spotButton;
  bool cleanButton;
  bool maxButton;
  signed int distanceSinceUpdate;
  signed int angleSinceUpdate;
  char chargeState;
  int voltage; //mV
  signed int current; //mA
  signed char temp; //Celsius
  unsigned int charge; //mAh
  unsigned int capacity; //mAh

 private:
  void initialize();
  
  int _rxPin;
  int _txPin;
  int _wakePin;
  char _sensors[26];

  int _defaultDuration;

};
#endif
