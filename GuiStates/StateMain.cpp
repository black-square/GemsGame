#include "stdafx.h"
#include "StateMain.h"
#include "StateAutoPlay.h"
#include "StateMainGame.h"
#include "StateZenGame.h"
#include "Gui/Widgets.h"

//////////////////////////////////////////////////////////////////////////
GuiStateMain::GuiStateMain()
{
  AddWidget( boost::make_shared<Gui::Image>(
    Rect( Point(0, 0), Size(755, 600) ),
    boost::make_shared<Texture>( "./_data/backg_main.png" )
    ));
 
  const Texture::TPtr pBtnTex = boost::make_shared<Texture>( "./_data/button_01.png", 2 );
  const Font::TPtr pBtnFont = boost::make_shared<Font>( "./_data/gm.ttf", 25);

  AddWidget( boost::make_shared<Gui::Label>(
    Point(377, 10),
    boost::make_shared<Font>( "./_data/gm.ttf", 150), 
    "The Game",
    Color::make_white(),
    Gui::Label::Center
  ));

  AddWidget( boost::make_shared<Gui::Label>(
    Point(377, 545),
    boost::make_shared<Font>( "./_data/gm.ttf", 50), 
    "Dmitry Shesterkin   dfb@yandex.ru",
    Color::make_white(),
    Gui::Label::Center
  ));

  AddWidget( boost::make_shared<Gui::Button>(
    Rect( Point(252, 220), Size(250, 50) ),
    pBtnFont, pBtnTex,
    BIND_THIS(StartMainGameState),
    "Start Game"
  ));

  AddWidget( boost::make_shared<Gui::Button>(
    Rect( Point(252, 300), Size(250, 50) ),
    pBtnFont, pBtnTex,
    BIND_THIS(StartZenGameState),
    "Zen Mode Game"
  ));

  AddWidget( boost::make_shared<Gui::Button>(
    Rect( Point(252, 380), Size(250, 50) ),
    pBtnFont, pBtnTex,
    BIND_THIS(StartAutoplayState),
    "Autoplay Test"
  ));

  m_pTex = boost::make_shared<Texture>( "./_data/explosion.png", 4 );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMain::StartAutoplayState()
{
  GetManager()->SetState( boost::make_shared<GuiStateAutoPlay>() );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMain::StartMainGameState()
{
  GetManager()->SetState( boost::make_shared<GuiStateMainGame>() );
}
//////////////////////////////////////////////////////////////////////////

void GuiStateMain::StartZenGameState()
{
  GetManager()->SetState( boost::make_shared<GuiStateZenGame>() );
}

void GuiStateMain::OnLButtonDown( Point pos )
{
  Particle::Param rgParams[4];

  const float scale = 2;
  const float lt = 0.3f;
  const float dl = 0.01f;

  rgParams[0].scaleEnd = scale; 
  rgParams[0].lifeTime = lt;
  rgParams[0].alphaMiddle = 0.7f;

  rgParams[1].hideDelay = dl;
  rgParams[1].scaleEnd = scale; 
  rgParams[1].lifeTime = lt + rgParams[1].hideDelay;
  rgParams[1].alphaMiddle = 0.7f;
  rgParams[1].texFrame = 1;

  rgParams[2].hideDelay = dl * 2;
  rgParams[2].scaleEnd = scale; 
  rgParams[2].lifeTime = lt + rgParams[2].hideDelay;
  rgParams[2].alphaMiddle = 0.7f;
  rgParams[2].texFrame = 2;

  rgParams[3].hideDelay = dl * 3;
  rgParams[3].scaleEnd = 1.5f * (scale); 
  rgParams[3].lifeTime = 1.5f * (lt + rgParams[3].hideDelay);
  rgParams[3].alphaMiddle = 0.7f;
  rgParams[3].texFrame = 3;


  m_part.Emit( pos, m_pTex, rgParams, ARRAY_SIZE(rgParams) );
}

void GuiStateMain::OnUpdate( float deltaTime )
{
  m_part.Update( deltaTime );
}

void GuiStateMain::OnRender() const
{
  m_part.Render();
}

