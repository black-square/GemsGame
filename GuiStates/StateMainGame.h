#ifndef StateMainGame_h__
#define StateMainGame_h__

#include "Engine/Gui/State.h"
#include "Logic/GameField.h"
#include "Logic/GameLogic.h"
#include "Engine/Gui/Widgets.h"
#include "Graphics/GameFieldRender.h"
#include "Engine/Core/TimeHelpers.hpp"

class GuiStateMainGame: public Gui::State
{
public:
  typedef GuiStateMainGame ThisType;

public:
  GuiStateMainGame();

private:
  void OnRender() const;
  void OnUpdate( float deltaTime );
  void OnLButtonDown( Point pos );
  void OnLButtonUp( Point pos );
  void OnMouseMove( Point pos );

private:
  void ReturnToMainState();
  void MatchesFound( GameLogic::TPoints &matches );

private:
  GameField m_field;
  GameLogic m_logic;
  GameFieldRender m_fieldRender;
  Texture::TPtr m_pTexBack;
  Gui::Image::TPtr m_pImgTimePlate;

  GameLogic::TMove m_hint;
  SimpleTimer<float> m_timerCheckMatches;
  SimpleTimer<float> m_timerShowHint;
  SimpleTimer<float> m_timerHintShowed;
  SimpleTimer<float> m_timerMultiplierReset;
  SimpleTimer<float> m_timerGameTime;
  boost::int64_t m_score;
  boost::int64_t m_scoreMultiplier;
  int            m_secRemain;
  Gui::Label::TPtr m_pLblScore;
  Gui::Label::TPtr m_pLblLastScore;
  Gui::Label::TPtr m_pLblMultiplier;
  Gui::Label::TPtr m_pLblTime;
  Gui::Label::TPtr m_pLblFinalText; 
};

#endif // StateMainGame_h__
