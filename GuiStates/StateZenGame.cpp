#include "stdafx.h"
#include "StateZenGame.h"
#include "Engine/Core/Interp.hpp"
#include "Engine/Audio/SoundManager.h"

const int g_gameDuration = 60;

GuiStateZenGame::GuiStateZenGame():
  m_logic(m_field),
  m_stepsCount(0),
  m_timerShowMatches(3.0f),
  m_timerHideMoves(3.0f)
{
  m_fieldRender.Init( Point(330, 100), 42 );

  m_logic.FillEmptyRandomly();
  m_logic.FindRemoveFillCompleate();
  m_logic.SetEventsHandler( &m_fieldRender );
  m_logic.FindAllMoves( m_possibleMoves );
  m_timerHideMoves.Start();

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

  const Font::TPtr pScoreFont = boost::make_shared<Font>( "./_data/gm.ttf", 33 );
  const Font::TPtr pHugeFont = boost::make_shared<Font>( "./_data/gm.ttf", 80 );

  const Point intPos(10, 70);
  const Point lblPosDiff(240, 25);
  const Size lblSize(0, 100);

  AddWidget( boost::make_shared<Gui::Label>(intPos, pScoreFont, "Steps:") );
  m_pLblStepsScore = boost::make_shared<Gui::Label>(intPos + lblPosDiff, pHugeFont, "", Color::make_white(), Gui::Label::Right );
  AddWidget( m_pLblStepsScore );

  m_pLblFinalText = boost::make_shared<Gui::Label>( Point(m_pTexBack->GetSize().w/2, m_pTexBack->GetSize().h/2 - 80), pHugeFont, "", Color::make_cyan(), Gui::Label::Center );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateZenGame::ReturnToMainState()
{
  GetManager()->SetMainState();
}
//////////////////////////////////////////////////////////////////////////

void GuiStateZenGame::OnRender( float deltaTime ) const
{
  if( !m_timerHideMoves.IsInProgress() )
  {
    BOOST_FOREACH( const GameLogic::TMove &cur, m_possibleMoves )
      m_fieldRender.RenderMark( cur.first );
  }

  m_fieldRender.Render();

  const Rect backRect( Point(303, 44), Point(689, 95) );
  Draw( *m_pTexBack, backRect, backRect );

  if( m_timerShowMatches.IsInProgress() )
  {
    BOOST_FOREACH( const Point &pt, m_matches )
      m_fieldRender.RenderMark(pt, 1);
  }

  m_pLblFinalText->Render();
}
//////////////////////////////////////////////////////////////////////////

void GuiStateZenGame::OnUpdate( float deltaTime )
{
  m_fieldRender.Update( deltaTime );
  m_timerShowMatches.Tick( deltaTime );
  m_timerHideMoves.Tick(deltaTime);
}
//////////////////////////////////////////////////////////////////////////

void GuiStateZenGame::OnLButtonDown( Point pos )
{
  if( !m_possibleMoves.empty() )
    m_fieldRender.LButtonDown( pos );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateZenGame::OnLButtonUp( Point pos )
{
  GameLogic::TMove move;
  m_fieldRender.LButtonUp( pos, move );

  if( move.first != move.second )
  { 
    if( !m_logic.MakeZenMove(move, m_matches) )
    {
      m_timerShowMatches.Start();
      m_timerHideMoves.Start();
    }
    else
    {
      ++m_stepsCount;
      m_pLblStepsScore->SetText( MakeString(FMT("%d") % m_stepsCount) );
      m_logic.FindAllMoves( m_possibleMoves );
      
      if( m_possibleMoves.empty() )
      {
        m_pLblFinalText->SetText( MakeString(FMT("You've done it!")), Color::make_cyan() );
        PlaySound( "./_data/tada.wav" );
      }
    }
  }
}
//////////////////////////////////////////////////////////////////////////

void GuiStateZenGame::OnMouseMove( Point pos )
{
  m_fieldRender.MouseMove( pos );
}


