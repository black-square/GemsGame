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

GameFieldRender::FallStateAccel *GameFieldRender::FallStateAccel::Start( PointF startPos, PointF destPos, float accel, float delay )
{
  const float elasticityCoefficient = 0.3f;
  const float dist = destPos.y - startPos.y;

  m_startPos = startPos;
  m_moveTime = 0;
  m_totalTime = std::sqrt( dist / accel );
  m_destPos = destPos;
  m_accel = accel;
  m_delay = delay;
  m_pState = m_delay > 0 ? &FallStateAccel::OnUpdateDelay : &FallStateAccel::OnUpdateBeforeBounce;
  m_speedAfterBounce = m_accel * m_totalTime * elasticityCoefficient;
   
  return this;   
}
//////////////////////////////////////////////////////////////////////////

bool GameFieldRender::FallStateAccel::OnUpdateDelay( float deltaTime, PointF &curPos )
{
  m_delay -= deltaTime;

  if( m_delay <= 0 )
  {
    m_moveTime += -m_delay;
    m_pState = &FallStateAccel::OnUpdateBeforeBounce; 
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////

bool GameFieldRender::FallStateAccel::OnUpdateBeforeBounce( float deltaTime, PointF &curPos )
{  
  m_moveTime += deltaTime;

  if( m_moveTime < m_totalTime )
  {
    curPos = m_startPos + PointF( 0, m_accel * m_moveTime * m_moveTime );
  }
  else
  {   
    m_moveTime -= m_totalTime;
    m_totalTime = m_speedAfterBounce / m_accel; //Maple: solve( {v*t = a*t^2}, {t} );
    m_pState = &FallStateAccel::OnUpdateAfterBounce;
  }  

  return true;
}
//////////////////////////////////////////////////////////////////////////

bool GameFieldRender::FallStateAccel::OnUpdateAfterBounce( float deltaTime, PointF &curPos )
{
  m_moveTime += deltaTime;
  
  if( m_moveTime < m_totalTime )
  {
    curPos = m_destPos + PointF( 0, m_accel * m_moveTime * m_moveTime - m_speedAfterBounce * m_moveTime );
    return true;
  }
  else
  {
    curPos = m_destPos;
    return false;  
  }
}
//////////////////////////////////////////////////////////////////////////

bool GameFieldRender::FallStateAccel::OnUpdate( float deltaTime, PointF &curPos )
{
  return (this->*m_pState)( deltaTime, curPos );
}
//////////////////////////////////////////////////////////////////////////

float GameFieldRender::FallStateAccel::CalcAccel( float distance, float time )
{
  return distance / ( time * time );   
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

void GameFieldRender::GemObj::FallTo( PointF destPos, float accel, float delay )
{
  m_pCurState = m_fallingState.Start( m_curPos, destPos, accel, delay );
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


