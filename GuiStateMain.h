#ifndef GuiStateMain_h__
#define GuiStateMain_h__

#include "GuiState.h"
#include "Texture.h"
#include "Font.h"

class GuiStateMain: public GuiState
{   
public:
  typedef GuiStateMain ThisType;

public:
  GuiStateMain();

private:
  void StartAutoplayState(); 
};


#endif // GuiStateMain_h__
