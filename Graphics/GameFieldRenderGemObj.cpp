#include "stdafx.h"
#include "GameFieldRenderGemObj.h"
#include "Core/Interp.hpp"

//////////////////////////////////////////////////////////////////////////

GameFieldRender::GemObj::GemObj( Point pos, const Texture &tex ):
   m_curPos(pos), m_destPos(pos), m_tex(tex)
{

}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::GemObj::Render() const
{
  Draw( m_tex, Point(m_curPos + PointBase<float>(0.5f, 0.5f)) );
}
//////////////////////////////////////////////////////////////////////////

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

  m_curPos = Lerp( m_startPos, m_destPos, SquaredStepFactor(m_moveTime / m_totlaTime) );
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::GemObj::MoveTo( Point destPos, float time )
{
  m_startPos = m_curPos;
  m_destPos = PointBase<float>(destPos);
  m_moveTime = 0;
  m_totlaTime = time;
}


