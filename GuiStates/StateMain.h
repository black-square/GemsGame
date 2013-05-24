#ifndef GuiStateMain_h__
#define GuiStateMain_h__

#include "Gui/State.h"


class GuiStateMain: public Gui::State
{   
public:
  typedef GuiStateMain ThisType;

public:
  GuiStateMain();

private:
  void StartAutoplayState();
  void StartMainGameState();
  void StartZenGameState();
};

#endif // GuiStateMain_h__
