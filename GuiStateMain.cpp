#include "stdafx.h"
#include "GuiStateMain.h"
#include "GuiWidgets.h"

GuiStateMain::GuiStateMain()
{
  AddWidget( boost::make_shared<GuiButton>(
    Rect( Point(20, 20), Size(200, 50) ),
    boost::make_shared<Font>( "./_data/gm.ttf", 25),
    boost::make_shared<Texture>( "./_data/button_01.png", 2 ),
    "The Button!"
  ));
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMain::OnRender()
{             
  RenderWidgets();
}


