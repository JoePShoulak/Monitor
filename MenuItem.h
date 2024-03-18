#ifndef MenuItem_h
#define MenuItem_h

#include <Arduino.h>

class MenuItem
{
public:
  MenuItem(long *options, int optionCount, String prefix = "");

  long getValue();
  void cycle();

  String getText();

private:
  long *_options;
  int _selectedIndex;
  int _optionCount;
  String _prefix;
};

#endif
