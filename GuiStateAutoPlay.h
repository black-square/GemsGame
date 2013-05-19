#ifndef GuiStateAutoPlay_h__
#define GuiStateAutoPlay_h__

#include "GuiState.h"
#include "Texture.h"
#include "GameField.h"
#include "GameFieldRender.h"
#include "GameLogic.h"
#include "TimeHelpers.hpp"
#include "Font.h"
#include "AutoPlay.h"
#include "GuiWidgets.h"

class GuiStateAutoPlay: public GuiState, private IGameLogicEvents
{
public:
  GuiStateAutoPlay();

private:
  void OnLButtonDown( point_t pos );
  void OnLButtonUp( point_t pos );
  void OnKeyDown( SDLKey sym, SDLMod mod, Uint16 unicode );
  void OnUpdate( float deltaTime );
  void OnRender( SDL_Surface *pDisplay );
  
  void OnSwap( point_t p1, point_t p2 );

private:
  GameField m_field;
  GameLogic m_logic;
  GameFieldRender m_fieldRender;
  GameFieldRender::PosOpt m_prevCellPos;
  AutoPlay m_autoPlay;
  SimpleTimer<float> m_autoPlayTimer;

  GuiLabel::TPtr m_pScore;

  int m_movesCount;
  Texture m_texTest;
};


#endif // GuiStateAutoPlay_h__
