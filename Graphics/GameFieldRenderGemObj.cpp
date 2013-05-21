#include "stdafx.h"
#include "GameFieldRenderGemObj.h"
#include "Core/Interp.hpp"

//////////////////////////////////////////////////////////////////////////

GameFieldRender::GemObj::GemObj( PointF pos, const Texture &tex ):
   m_curPos(pos), m_destPos(pos), m_tex(tex)
{

}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::GemObj::Render() const
{
  Draw( m_tex, Round(m_curPos) );
}
//////////////////////////////////////////////////////////////////////////

static SquaredWithBounceStepFactor<float> g_lerpFactor( 0.8f, 0.93f );

void GameFieldRender::GemObj::Update( float deltaTime )
{
  if( m_curPos == m_destPos )
    return;

  m_moveTime += deltaTime;

  if( m_moveTime >= m_totlaTime )
  {
    m_curPos = m_destPos;
    return;  
  }

  m_curPos = Lerp( m_startPos, m_destPos, g_lerpFactor(m_moveTime / m_totlaTime) );
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::GemObj::MoveTo( PointF destPos, float time )
{
  m_startPos = m_curPos;
  m_destPos = destPos;
  m_moveTime = 0;
  m_totlaTime = time;
}


