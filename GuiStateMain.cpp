#include "stdafx.h"
#include "GuiStateMain.h"
#include "GuiWidgets.h"
#include "GuiStateAutoPlay.h"

GuiStateMain::GuiStateMain()
{
  AddWidget( boost::make_shared<GuiButton>(
    Rect( Point(290, 220), Size(200, 50) ),
    boost::make_shared<Font>( "./_data/gm.ttf", 25),
    boost::make_shared<Texture>( "./_data/button_01.png", 2 ),
    BIND_THIS(StartAutoplayState),
    "Autoplay Test"
  ));
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMain::StartAutoplayState()
{
  GetManager()->SetState( boost::make_shared<GuiStateAutoPlay>() );
}
//////////////////////////////////////////////////////////////////////////



