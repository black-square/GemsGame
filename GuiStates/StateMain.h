#ifndef GuiStateMain_h__
#define GuiStateMain_h__

#include "Gui/State.h"
#include "Graphics/Texture.h"
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
  
private:
  void OnRender() const;
  void OnLButtonDown( Point pos );

  virtual void OnUpdate( float deltaTime );

private:
  Texture::TPtr m_pTexTst;
  ParticlesManager m_particles;
};

#endif // GuiStateMain_h__
