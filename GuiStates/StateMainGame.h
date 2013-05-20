#ifndef StateMainGame_h__
#define StateMainGame_h__

#include "Gui/State.h"
#include "Logic/GameField.h"
#include "Logic/GameLogic.h"
#include "Gui/Widgets.h"
#include "Graphics/GameFieldRender.h"

class GuiStateMainGame: public Gui::State
{
public:
  typedef GuiStateMainGame ThisType;

public:
  GuiStateMainGame();

private:
  void OnRender() const;

private:
  void ReturnToMainState();

  virtual void OnUpdate( float deltaTime );


private:
  GameField m_field;
  GameLogic m_logic;
  GameFieldRender m_fieldRender;
  Texture::TPtr m_pTexBack;

};

#endif // StateMainGame_h__
