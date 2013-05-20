#include "stdafx.h"
#include "StateMainGame.h"

GuiStateMainGame::GuiStateMainGame():
  m_fieldRender(m_field)
{
  m_logic.FillEmptyRandomly( m_field );
  m_fieldRender.Init( Point(330, 100), 42 );

  AddWidget( boost::make_shared<Gui::Image>( Point(0, 0), boost::make_shared<Texture>("./_data/background.jpg") ) );

  const Font::TPtr pBtnFont = boost::make_shared<Font>( "./_data/gm.ttf", 17);
  const Texture::TPtr pBtnTex = boost::make_shared<Texture>( "./_data/button_01.png", 2 );

  AddWidget( boost::make_shared<Gui::Button>(
    Rect( Point(650, 5), Size(100, 25) ),
    pBtnFont, pBtnTex,
    BIND_THIS(ReturnToMainState),
    "Main Menu"
  ));
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::ReturnToMainState()
{
  GetManager()->SetMainState();
}

void GuiStateMainGame::OnRender()
{
  m_fieldRender.Render();
}
