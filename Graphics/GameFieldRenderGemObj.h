#ifndef GameFieldRenderGem_h__
#define GameFieldRenderGem_h__

#include "GameFieldRender.h"
#include "Core/Interp.hpp"

struct GameFieldRender::IGemState
{
  virtual ~IGemState () {}
  virtual bool OnUpdate( float deltaTime, PointF &curPos ) = 0; 
};
//////////////////////////////////////////////////////////////////////////

class GameFieldRender::DefaultState: public IGemState
{
public:
  DefaultState *Start() { return this; }
  bool OnUpdate( float deltaTime, PointF &curPos ) { return true; }
};
//////////////////////////////////////////////////////////////////////////

class GameFieldRender::FallState: public IGemState
{
public:
  FallState *Start( PointF startPos, PointF destPos, float totalTime );
  bool OnUpdate( float deltaTime, PointF &curPos );

private:
  PointF m_startPos;
  float m_moveTime;
  float m_totalTime;
  PointF m_destPos;
  static const SquaredWithBounceStepFactor<float> lerpFactor;
};
//////////////////////////////////////////////////////////////////////////

class GameFieldRender::SpringState: public IGemState
{
public:
  SpringState *Start( PointF destPos );
  bool OnUpdate( float deltaTime, PointF &curPos );
  void UpdateDestPos( PointF destPos );
   
private:
  PointF m_destPos;
  PointF m_velocity;
};
//////////////////////////////////////////////////////////////////////////

class GameFieldRender::FallStateAccel: public IGemState
{
public:
  FallStateAccel *Start( PointF startPos, PointF destPos, float accel, float delay );
  bool OnUpdate( float deltaTime, PointF &curPos );
  static float CalcAccel(float distance, float time );

private:
  typedef bool (FallStateAccel::*UpdateStateT)( float deltaTime, PointF &curPos );
  
private:
  bool OnUpdateDelay( float deltaTime, PointF &curPos );
  bool OnUpdateBeforeBounce( float deltaTime, PointF &curPos );
  bool OnUpdateAfterBounce( float deltaTime, PointF &curPos );
  
private:
  PointF m_startPos;
  float m_moveTime;
  float m_totalTime;
  PointF m_destPos;
  float m_delay;
  float m_accel;
  float m_speedAfterBounce;
  UpdateStateT m_pState;
};
//////////////////////////////////////////////////////////////////////////

class GameFieldRender::GemObj
{
public:
  GemObj( PointF pos, const Texture &tex );
  void Render() const;
  void Update( float deltaTime );
  void FallTo( PointF destPos, float accel, float delay );
  void UpdateDragPoint( PointF destPos );
  void SetPos( PointF curPos );

private:
  PointF m_curPos;
  const Texture &m_tex;
  
  DefaultState m_defaultState;
  FallStateAccel m_fallingState;
  SpringState m_springState;

  IGemState *m_pCurState;
};

#endif // GameFieldRenderGem_h__