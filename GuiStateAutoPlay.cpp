#include "stdafx.h"
#include "GuiStateAutoPlay.h"
#include "GuiWidgets.h"

GuiStateAutoPlay::GuiStateAutoPlay():
  m_fieldRender(m_field), m_movesCount(0)
{
  m_logic.FillEmptyRandomly( m_field );
  m_logic.SetEventsHandler( this );

  AddWidget( boost::make_shared<GuiImage>( Point(0, 0), boost::make_shared<Texture>("./_data/background.jpg") ) );
  
  m_fieldRender.Init( Point(330, 100), 42 );

  m_pScore = boost::make_shared<GuiLabel>( Point(38, 120), boost::make_shared<Font>( "./_data/gm.ttf", 25), "No moves" );
  AddWidget( m_pScore );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::OnLButtonDown( Point pos )
{
  m_prevCellPos = m_fieldRender.GetGemPos( pos );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::OnLButtonUp( Point pos )
{
  GameFieldRender::PosOpt curCellPos =  m_fieldRender.GetGemPos( pos );

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
  if( sym == SDLK_a )
    if( !m_autoPlayTimer.IsInProgress() ) 
      m_autoPlayTimer.Start( 0.001f );
    else
      m_autoPlayTimer.Stop();

  if( sym == SDLK_SPACE )
    m_autoPlay.Update( m_field, m_logic );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::OnUpdate( float deltaTime )
{
  if( m_autoPlayTimer.TickWithRestart(deltaTime) )
    m_autoPlay.Update( m_field, m_logic ); 
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::OnRender()
{
  RenderWidgets();
  
  BOOST_FOREACH( const Point &cur, m_autoPlay.GetMarks() )
    m_fieldRender.RenderMark( cur ); 

  m_fieldRender.Render();
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::OnSwap( Point p1, Point p2 )
{
  ++m_movesCount;
  m_pScore->SetText( MakeString(FMT("Moves count: %d") % m_movesCount) ); 
}
