#include "stdafx.h"
#include "GuiStateAutoPlay.h"
#include "GuiWidgets.h"

GuiStateAutoPlay::GuiStateAutoPlay():
  m_fieldRender(m_field), m_movesCount(0)
{
  m_logic.FillEmptyRandomly( m_field );
  m_logic.SetEventsHandler( this );

  AddWidget( boost::make_shared<GuiImage>( point_t(0, 0), boost::make_shared<Texture>("./_data/background.jpg") ) );
  
  m_fieldRender.Init( point_t(330, 100), 42 );

  m_pScore = boost::make_shared<GuiLabel>( point_t(38, 120), boost::make_shared<Font>( "./_data/gm.ttf", 25), "No moves" );
  AddWidget( m_pScore );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::OnLButtonDown( point_t pos )
{
  m_prevCellPos = m_fieldRender.GetGemPos( pos );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::OnLButtonUp( point_t pos )
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

void GuiStateAutoPlay::OnRender( SDL_Surface *pDisplay )
{
  RenderWidgets( pDisplay );
  
  BOOST_FOREACH( const point_t &cur, m_autoPlay.GetMarks() )
    m_fieldRender.RenderMark( pDisplay, cur ); 

  m_fieldRender.Render( pDisplay );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateAutoPlay::OnSwap( point_t p1, point_t p2 )
{
  ++m_movesCount;
  m_pScore->SetText( MakeString(FMT("Moves count: %d") % m_movesCount) ); 
}
