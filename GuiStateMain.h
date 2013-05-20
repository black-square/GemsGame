#ifndef GuiStateMain_h__
#define GuiStateMain_h__

#include "GuiState.h"
#include "Texture.h"
#include "Font.h"

class GuiStateMain: public GuiState
{
public:
  GuiStateMain();

private:
  void OnRender(); 
};


#endif // GuiStateMain_h__
