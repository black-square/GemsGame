#ifndef GuiStateAutoPlay_h__
#define GuiStateAutoPlay_h__

#include "Gui/State.h"
#include "Logic/GameField.h"
#include "Logic/GameLogic.h"
#include "Graphics/SimpleGameFieldRender.h"
#include "Core/TimeHelpers.hpp"
#include "Logic/AutoPlay.h"
#include "Gui/Widgets.h"

class GuiStateAutoPlay: public Gui::State, private GameLogic::IEvents
{
public:
  typedef GuiStateAutoPlay ThisType;

public:
  GuiStateAutoPlay();

private:
  void OnLButtonDown( Point pos );
  void OnLButtonUp( Point pos );
  void OnKeyDown( SDLKey sym, SDLMod mod, Uint16 unicode );
  void OnUpdate( float deltaTime );
  void OnRender() const;
  
  void OnGemSwap( Point p1, Point p2 );

private:
  void ReturnToMainState();
  void DoStep();
  void DoAuto( float deltaTime );

private:
  GameField m_field;
  GameLogic m_logic;
  SimpleGameFieldRender m_fieldRender;
  SimpleGameFieldRender::PosOpt m_prevCellPos;
  AutoPlay m_autoPlay;
  SimpleTimer<float> m_autoPlayTimer;

  Gui::Label::TPtr m_pScore;

  int m_movesCount;
};


#endif // GuiStateAutoPlay_h__
