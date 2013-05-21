#include "stdafx.h"
#include "GameFieldRenderGemObj.h"

//////////////////////////////////////////////////////////////////////////

const SquaredWithBounceStepFactor<float> GameFieldRender::FallState::lerpFactor( 0.8f, 0.93f );
//////////////////////////////////////////////////////////////////////////

GameFieldRender::FallState *GameFieldRender::FallState::Start( PointF startPos, PointF destPos, float totalTime )
{
  m_startPos = startPos;
  m_destPos = destPos;
  m_moveTime = 0;
  m_totalTime = totalTime;

  return this;
}
//////////////////////////////////////////////////////////////////////////

bool GameFieldRender::FallState::OnUpdate( float deltaTime, PointF &curPos )
{
  if( curPos == m_destPos )
    return false;

  m_moveTime += deltaTime;

  if( m_moveTime >= m_totalTime )
  {
    curPos = m_destPos;
    return false;  
  }

  curPos = Lerp( m_startPos, m_destPos, lerpFactor(m_moveTime / m_totalTime) );
  return true;
}
//////////////////////////////////////////////////////////////////////////

GameFieldRender::SpringState *GameFieldRender::SpringState::Start( PointF destPos )
{
  m_destPos = destPos;
  m_velocity.zeroize();
  return this;
}
//////////////////////////////////////////////////////////////////////////

bool GameFieldRender::SpringState::OnUpdate( float deltaTime, PointF &curPos )
{
  curPos = SmoothCritDamp( curPos, m_destPos, m_velocity, deltaTime, 0.1f );
  return curPos != m_destPos;
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::SpringState::UpdateDestPos( PointF destPos )
{
  m_destPos = destPos;
}
//////////////////////////////////////////////////////////////////////////

GameFieldRender::GemObj::GemObj( PointF pos, const Texture &tex ):
   m_curPos(pos), m_tex(tex), m_pCurState( m_defaultState.Start() )
{
  //Nothing
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::GemObj::Render() const
{
  Draw( m_tex, Round(m_curPos) );
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::GemObj::Update( float deltaTime )
{
  if( !m_pCurState->OnUpdate(deltaTime, m_curPos) )
    m_pCurState = m_defaultState.Start();
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::GemObj::FallTo( PointF destPos, float totalTime )
{
  m_pCurState = m_fallingState.Start( m_curPos, destPos, totalTime );
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::GemObj::UpdateDragPoint( PointF destPos )
{
  if( m_pCurState != &m_springState )
    m_pCurState = m_springState.Start( m_curPos );
  
  m_springState.UpdateDestPos( destPos );
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::GemObj::SetPos( PointF curPos )
{
  m_curPos = curPos;
  m_pCurState = m_defaultState.Start();  
}


