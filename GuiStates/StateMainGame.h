#ifndef StateMainGame_h__
#define StateMainGame_h__

#include "Gui/State.h"
#include "Logic/GameField.h"
#include "Logic/GameLogic.h"
#include "Gui/Widgets.h"
#include "Graphics/GameFieldRender.h"
#include "Core/TimeHelpers.hpp"

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

private:
  GameField m_field;
  GameLogic m_logic;
  GameFieldRender m_fieldRender;
  Texture::TPtr m_pTexBack;
  GameLogic::TMove m_hint;
  SimpleTimer<float> m_timerCheckMatches;
  SimpleTimer<float> m_timerShowHint;
  SimpleTimer<float> m_timerHintShowed;
};

#endif // StateMainGame_h__
