#include "stdafx.h"
#include "GuiStates/StateAutoPlay.h"
#include "Gui/Widgets.h"

GuiStateAutoPlay::GuiStateAutoPlay():
  m_fieldRender(m_field), m_movesCount(0)
{
  m_logic.FillEmptyRandomly( m_field );
  m_logic.SetEventsHandler( this );

  AddWidget( boost::make_shared<Gui::Image>( Point(0, 0), boost::make_shared<Texture>("./_data/background.jpg") ) );
  
  m_fieldRender.Init( Point(330, 100), 42 );

  m_pScore = boost::make_shared<Gui::Label>( Point(38, 120), boost::make_shared<Font>( "./_data/gm.ttf", 25), "No moves" );
  AddWidget( m_pScore );

  const Font::TPtr pBtnFont = boost::make_shared<Font>( "./_data/gm.ttf", 17);
  const Texture::TPtr pBtnTex = boost::make_shared<Texture>( "./_data/button_01.png", 2 );

  AddWidget( boost::make_shared<Gui::Button>(
    Rect( Point(650, 5), Size(100, 25) ),
    pBtnFont, pBtnTex,
    BIND_THIS(ReturnToMainState),
    "Main Menu"
  ));

  AddWidget( boost::make_shared<Gui::Button>(
    Rect( Point(240, 530), Size(150, 25) ),
    pBtnFont, pBtnTex,
    BIND_THIS(DoStep),
     "Step (Space)"
  ));

  AddWidget( boost::make_shared<Gui::Button>(
    Rect( Point(400, 530), Size(150, 25) ),
    pBtnFont, pBtnTex,
    BIND_THIS(DoAuto, 0.5f),
    "Auto"
  ));

  AddWidget( boost::make_shared<Gui::Button>(
    Rect( Point(560, 530), Size(150, 25) ),
    pBtnFont, pBtnTex,
    BIND_THIS(DoAuto, 0.001f),
    "Auto Hight Speed"
  ));
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::DoStep()
{
  m_autoPlay.Update( m_field, m_logic );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::DoAuto( float deltaTime )
{
  if( !m_autoPlayTimer.IsInProgress() ) 
    m_autoPlayTimer.Start( deltaTime );
  else
    m_autoPlayTimer.Stop();
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::ReturnToMainState()
{
  GetManager()->SetMainState();
}

void GuiStateAutoPlay::OnLButtonDown( Point pos )
{
  m_prevCellPos = m_fieldRender.GetGemPos( pos );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::OnLButtonUp( Point pos )
{
  SimpleGameFieldRender::PosOpt curCellPos =  m_fieldRender.GetGemPos( pos );

  if( m_prevCellPos && curCellPos && *m_prevCellPos != *curCellPos )
    if( manhattanDistance(*m_prevCellPos, *curCellPos) == 1 )
    {
      m_logic.Swap( m_field, *m_prevCellPos, *curCellPos );

      m_autoPlay.Reset();
      m_autoPlay.Update( m_field, m_logic );
    }

    m_prevCellPos = boost::none; 
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::OnKeyDown( SDLKey sym, SDLMod mod, Uint16 unicode )
{
  if( sym == SDLK_SPACE )
    DoStep();
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::OnUpdate( float deltaTime )
{
  if( m_autoPlayTimer.TickWithRestart(deltaTime) )
    m_autoPlay.Update( m_field, m_logic ); 
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::OnRender() const
{  
  BOOST_FOREACH( const Point &cur, m_autoPlay.GetMarks() )
    m_fieldRender.RenderMark( cur ); 

  m_fieldRender.Render();
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::OnGemSwap( Point p1, Point p2 )
{
  ++m_movesCount;
  m_pScore->SetText( MakeString(FMT("Moves count: %d") % m_movesCount) ); 
}
