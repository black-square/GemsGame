#ifndef StateZenGame_h__
#define StateZenGame_h__

#include "Gui/State.h"
#include "Logic/GameField.h"
#include "Logic/GameLogic.h"
#include "Gui/Widgets.h"
#include "Graphics/GameFieldRender.h"
#include "Core/TimeHelpers.hpp"

class GuiStateZenGame: public Gui::State
{
public:
  typedef GuiStateZenGame ThisType;

public:
  GuiStateZenGame();

private:
  void OnRender() const;
  void OnUpdate( float deltaTime );
  void OnLButtonDown( Point pos );
  void OnLButtonUp( Point pos );
  void OnMouseMove( Point pos );

private:
  void ReturnToMainState();

private:
  GameField m_field;
  GameLogic m_logic;
  GameFieldRender m_fieldRender;
  Texture::TPtr m_pTexBack;
  boost::int64_t m_stepsCount;
  Gui::Label::TPtr m_pLblStepsScore;
  Gui::Label::TPtr m_pLblFinalText;
  SimpleTimer<float> m_timerShowMatches;
  SimpleTimer<float> m_timerHideMoves;
  GameLogic::TPoints m_matches;
  GameLogic::TMoves m_possibleMoves;
};

#endif // StateZenGame_h__