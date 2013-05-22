#include "stdafx.h"
#include "StateMainGame.h"
#include "Core/Interp.hpp"

const int g_gameDuration = 60;

GuiStateMainGame::GuiStateMainGame():
  m_logic(m_field),
  m_timerCheckMatches( 1.0f ),
  m_timerShowHint( 10.0f ),
  m_timerHintShowed( 3.0f ),
  m_timerMultiplierReset( 3.0f ),
  m_timerGameTime(1.0f),
  m_score(0),
  m_scoreMultiplier(1),
  m_secRemain(g_gameDuration)
{
  m_fieldRender.Init( Point(330, 100), 42 );
 
  m_logic.FillEmptyRandomly();
  m_logic.FindRemoveFillCompleate();
  m_logic.SetEventsHandler( &m_fieldRender );

  m_timerShowHint.Start();
  m_timerGameTime.Start();

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

  m_pLblLastScore = boost::make_shared<Gui::Label>( Point(550, 5), pScoreFont, "", Color::make_white(), Gui::Label::Right );
  AddWidget( m_pLblLastScore );

  const Point intPos(10, 70);
  const Point lblPosDiff(240, 25);
  const Size lblSize(0, 100);

  AddWidget( boost::make_shared<Gui::Label>(intPos, pScoreFont, "Score:") );
  m_pLblScore = boost::make_shared<Gui::Label>(intPos + lblPosDiff, pHugeFont, "", Color::make_white(), Gui::Label::Right );
  AddWidget( m_pLblScore );

  AddWidget( boost::make_shared<Gui::Label>(intPos + lblSize, pScoreFont, "Multiplier:") );
  m_pLblMultiplier = boost::make_shared<Gui::Label>( intPos + lblSize + lblPosDiff, pHugeFont, "", Color::make_white(), Gui::Label::Right );
  AddWidget( m_pLblMultiplier );

  m_pLblTime = boost::make_shared<Gui::Label>( Point(117, 430), pHugeFont, "", Color::make_white(), Gui::Label::Center );
  AddWidget( m_pLblTime );

  m_pLblFinalText = boost::make_shared<Gui::Label>( Point(m_pTexBack->GetSize().w/2, m_pTexBack->GetSize().h/2 - 80), pHugeFont, "", Color::make_cyan(), Gui::Label::Center );
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

  const Rect backRect( Point(303, 44), Point(689, 95) );
  Draw( *m_pTexBack, backRect, backRect );

  if( m_timerHintShowed.IsInProgress() )
  {
    m_fieldRender.RenderMark( m_hint.first );
    m_fieldRender.RenderMark( m_hint.second );
  }  

  m_pLblFinalText->Render();
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::MatchesFound( GameLogic::TPoints &matches )
{
  const boost::int64_t curScore =  matches.size() * 50; 
  m_score += m_scoreMultiplier *curScore;
  ++m_scoreMultiplier;
  m_timerMultiplierReset.Start(); 

  m_pLblScore->SetText( MakeString(FMT("%d") % m_score) );
  m_pLblLastScore->SetText( MakeString(FMT("%d x %d") % m_scoreMultiplier % curScore), Color::make_yellow() );
  m_pLblMultiplier->SetText( MakeString(FMT("x %d") % m_scoreMultiplier) );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::OnUpdate( float deltaTime )
{
  m_fieldRender.Update( deltaTime );

  if( m_timerCheckMatches.Tick(deltaTime) )
  {
    GameLogic::TPoints matches;

    if( m_logic.FindRemoveFillOnce(matches) )
    {
      MatchesFound(matches);
      m_timerCheckMatches.Start();
    }  
  }

  if( m_timerShowHint.TickWithRestart(deltaTime) )
  {
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
  
  if( m_timerMultiplierReset.Tick(deltaTime) )
  {
    m_scoreMultiplier = 1;
    m_pLblMultiplier->SetText( MakeString(FMT("x %d") % m_scoreMultiplier) );
    m_pLblLastScore->SetText( "" );
  }

  if( m_timerGameTime.TickWithRestart(deltaTime) )
  {
    m_pLblTime->SetText( MakeString(FMT("%d") % m_secRemain), MakeColorFromHSV( Lerp(0.f, 120.f, float(m_secRemain) / g_gameDuration), 1, 1) );

    if( --m_secRemain < 0 )
    {
      m_timerGameTime.Stop();
      m_pLblFinalText->SetText( MakeString(FMT("Time is Up!")), Color::make_cyan() );
    }  
  }
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::OnLButtonDown( Point pos )
{
  if( m_secRemain >= 0 )
    m_fieldRender.LButtonDown( pos );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::OnLButtonUp( Point pos )
{
  GameLogic::TMove move;
  m_fieldRender.LButtonUp( pos, move );

  if( move.first != move.second )
  {
    GameLogic::TPoints matches;

    if( m_logic.MakeMove(move, matches) )
    {
      MatchesFound(matches);
      m_timerCheckMatches.Start();
      m_timerShowHint.Start();
      m_timerHintShowed.Stop();
    }
  }
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMainGame::OnMouseMove( Point pos )
{
  m_fieldRender.MouseMove( pos );
}
