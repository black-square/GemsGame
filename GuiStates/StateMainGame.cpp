#include "stdafx.h"
#include "StateMainGame.h"

GuiStateMainGame::GuiStateMainGame():
  m_fieldRender()
{
  m_fieldRender.Init( Point(330, 100), 42 );
  m_logic.SetEventsHandler( &m_fieldRender );
  m_logic.FillEmptyRandomly( m_field );
  
  m_pTexBack = boost::make_shared<Texture>("./_data/background.jpg");

  AddWidget( boost::make_shared<Gui::Image>( Point(0, 0), m_pTexBack ) );

  const Font::TPtr pBtnFont = boost::make_shared<Font>( "./_data/gm.ttf", 17 );
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

void GuiStateMainGame::OnRender() const
{
  m_fieldRender.Render();

  const Rect backRect( Point(303, 0), Point(689, 95) );
  Draw( *m_pTexBack, backRect, backRect );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::OnUpdate( float deltaTime )
{
  m_fieldRender.Update( deltaTime );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::OnLButtonDown( Point pos )
{
  m_fieldRender.LButtonDown( pos );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::OnLButtonUp( Point pos )
{
  m_fieldRender.LButtonUp( pos );
}
//////////////////////////////////////////////////////////////////////////\

void GuiStateMainGame::OnMouseMove( Point pos )
{
  m_fieldRender.MouseMove( pos );
}
