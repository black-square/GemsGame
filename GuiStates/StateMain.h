#ifndef GuiStateMain_h__
#define GuiStateMain_h__

#include "Gui/State.h"
#include "Particles/ParticlesManager.h"

class GuiStateMain: public Gui::State
{   
public:
  typedef GuiStateMain ThisType;

public:
  GuiStateMain();

private:
  void StartAutoplayState();
  void StartMainGameState();
  void StartZenGameState();

  virtual void OnLButtonDown( Point pos );

  virtual void OnUpdate( float deltaTime );

  virtual void OnRender() const;

private:
  ParticlesManager m_part;
  Texture::TPtr m_pTex;
};

#endif // GuiStateMain_h__
