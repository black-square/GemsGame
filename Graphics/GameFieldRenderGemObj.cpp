#include "stdafx.h"
#include "GameFieldRenderGemObj.h"
#include "Audio/SoundManager.h"

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

float GameFieldRender::FallingGemsManager::FromPointDelay( Point from )
{
  const float fieldSize = GameField::FieldSize - 1.f;
  return MinDelayCoeff() * ( fieldSize - from.y - from.x );
}
//////////////////////////////////////////////////////////////////////////

GameFieldRender::FallParams GameFieldRender::FallingGemsManager::NextParamsOutOfField( Point from, int cellSize )
{ 
  FallParams params;

  params.accel = FallState::CalcAccel( float(cellSize), GetFirstCellTime() );
  params.delay = GetFirstCellTime() * m_flyingGemsCount[from.x]++ + FromPointDelay( from );

  return params;
}
//////////////////////////////////////////////////////////////////////////

GameFieldRender::FallParams GameFieldRender::FallingGemsManager::NextParamInField( Point from, int cellSize )
{
  FallParams params;

  params.accel = FallState::CalcAccel( float(cellSize), GetFirstCellTime() );
  params.delay = FromPointDelay( from );

  return params;
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::FallingGemsManager::Reset()
{
  std::fill_n( m_flyingGemsCount, ARRAY_SIZE(m_flyingGemsCount), 0 );
}
//////////////////////////////////////////////////////////////////////////

GameFieldRender::FallState *GameFieldRender::FallState::Start( PointF startPos, PointF destPos, float accel, float delay )
{
  const float elasticityCoefficient = 0.3f;
  const float dist = destPos.y - startPos.y;

  m_startPos = startPos;
  m_moveTime = 0;
  m_totalTime = std::sqrt( dist / accel );
  m_destPos = destPos;
  m_accel = accel;
  m_delay = delay;
  m_pState = m_delay > 0 ? &FallState::OnUpdateDelay : &FallState::OnUpdateBeforeBounce;
  m_speedAfterBounce = m_accel * m_totalTime * elasticityCoefficient;
   
  return this;   
}
//////////////////////////////////////////////////////////////////////////

bool GameFieldRender::FallState::OnUpdateDelay( float deltaTime, PointF &curPos )
{
  m_delay -= deltaTime;

  if( m_delay <= 0 )
  {
    m_moveTime += -m_delay;
    m_pState = &FallState::OnUpdateBeforeBounce; 
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////

bool GameFieldRender::FallState::OnUpdateBeforeBounce( float deltaTime, PointF &curPos )
{  
  m_moveTime += deltaTime;

  if( m_moveTime < m_totalTime )
  {
    curPos = m_startPos + PointF( 0, m_accel * m_moveTime * m_moveTime );
  }
  else
  {   
    curPos = m_destPos;
    m_moveTime -= m_totalTime;
    m_totalTime = m_speedAfterBounce / m_accel; //Maple: solve( {a*t^2 - v*t = 0}, {t} );
    m_pState = &FallState::OnUpdateAfterBounce;
    PlaySound("./_data/gem_fall.wav" );
  }  

  return true;
}
//////////////////////////////////////////////////////////////////////////

bool GameFieldRender::FallState::OnUpdateAfterBounce( float deltaTime, PointF &curPos )
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

bool GameFieldRender::FallState::OnUpdate( float deltaTime, PointF &curPos )
{
  return (this->*m_pState)( deltaTime, curPos );
}
//////////////////////////////////////////////////////////////////////////

float GameFieldRender::FallState::CalcAccel( float distance, float time )
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

void GameFieldRender::GemObj::FallTo( PointF destPos, const FallParams &fallParams )
{
  m_pCurState = m_fallingState.Start( m_curPos, destPos, fallParams.accel, fallParams.delay );
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


