#include "stdafx.h"
#include "GuiStateMain.h"

GuiStateMain::GuiStateMain()
{
  m_texTest.Load( "./_data/button_01.png", 2 );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMain::OnRender( SDL_Surface *pDisplay )
{
  //Draw( m_texTest, rect_t( 20, 20, 600, 100 ) );
  Draw( m_texTest, point_t( 20, 20 ), 1 );
}


