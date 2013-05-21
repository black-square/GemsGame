#include "stdafx.h"
#include "StateMain.h"
#include "StateAutoPlay.h"
#include "StateMainGame.h"
#include "Gui/Widgets.h"

//////////////////////////////////////////////////////////////////////////
GuiStateMain::GuiStateMain()
{
  AddWidget( boost::make_shared<Gui::Button>(
    Rect( Point(290, 220), Size(200, 50) ),
    boost::make_shared<Font>( "./_data/gm.ttf", 25),
    boost::make_shared<Texture>( "./_data/button_01.png", 2 ),
    BIND_THIS(StartMainGameState),
    "Start Game"
  ));

  AddWidget( boost::make_shared<Gui::Button>(
    Rect( Point(290, 300), Size(200, 50) ),
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

void GuiStateMain::StartMainGameState()
{
  GetManager()->SetState( boost::make_shared<GuiStateMainGame>() );
}
//////////////////////////////////////////////////////////////////////////


