#include "stdafx.h"
#include "StateMainGame.h"

GuiStateMainGame::GuiStateMainGame():
  m_logic(m_field),
  m_timerCheckMatches( 1.0f ),
  m_timerShowHint( 10.0f ),
  m_timerHintShowed( 3.0f )
{
  m_fieldRender.Init( Point(330, 100), 42 );
 
  m_logic.FillEmptyRandomly();
  m_logic.FindRemoveFillCompleate();
  m_logic.SetEventsHandler( &m_fieldRender );

  m_timerShowHint.Start();

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
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::OnRender() const
{
  m_fieldRender.Render();

  const Rect backRect( Point(303, 0), Point(689, 95) );
  Draw( *m_pTexBack, backRect, backRect );

  if( m_timerHintShowed.IsInProgress() )
  {
    m_fieldRender.RenderMark( m_hint.first );
    m_fieldRender.RenderMark( m_hint.second );
  }  
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::OnUpdate( float deltaTime )
{
  m_fieldRender.Update( deltaTime );

  if( m_timerCheckMatches.Tick(deltaTime) )
    if( m_logic.FindRemoveFillOnce() )
      m_timerCheckMatches.Start();  

  if( m_timerShowHint.Tick(deltaTime) )
  {
    m_timerShowHint.Start();

    GameLogic::TMoves moves;
    m_logic.FindAllMoves( moves );

    if( !moves.empty() )
    {
      m_hint = m_logic.GetRand( moves );
      m_timerHintShowed.Start();  
    }
    else
    {
      m_logic.RecreateField();
      m_timerCheckMatches.Start();
    }   
  }

  m_timerHintShowed.Tick( deltaTime );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::OnLButtonDown( Point pos )
{
  m_fieldRender.LButtonDown( pos );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::OnLButtonUp( Point pos )
{
  GameLogic::TMove move;
  m_fieldRender.LButtonUp( pos, move );

  if( move.first != move.second )
    if( m_logic.MakeMove(move) )
    {
      m_timerCheckMatches.Start();
      m_timerShowHint.Start();
      m_timerHintShowed.Stop();
    }
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::OnMouseMove( Point pos )
{
  m_fieldRender.MouseMove( pos );
}
