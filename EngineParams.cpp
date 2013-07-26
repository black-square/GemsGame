#include "stdafx.h"
#include "Engine/IEngineParams.h"
#include "GuiStates/StateMain.h"

struct EngineParamsImpl: IEngineParams
{
  Size GetInitialScreenSize() const override
  {
    return Size(755, 600);  
  }

  Gui::State::TPtr MakeMainState() const override
  {
    return boost::make_shared<GuiStateMain>();
  }
};
//////////////////////////////////////////////////////////////////////////

IEngineParams *EngineParams()
{
  static EngineParamsImpl params;
  return &params; 
}



