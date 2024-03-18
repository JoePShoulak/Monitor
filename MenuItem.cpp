#include "MenuItem.h"

MenuItem::MenuItem(long *options, int optionCount, String prefix = "")
{
  _options = options;
  _prefix = prefix;
  _optionCount = optionCount;
  _selectedIndex = 0;
}

long MenuItem::getValue()
{
  return _options[_selectedIndex];
}

String MenuItem::getText()
{
  return _prefix + this->getValue();
}

void MenuItem::cycle()
{
  _selectedIndex = (_selectedIndex + 1) % _optionCount;
}
