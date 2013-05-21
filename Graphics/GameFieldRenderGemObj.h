#ifndef GameFieldRenderGem_h__
#define GameFieldRenderGem_h__

#include "GameFieldRender.h"

class GameFieldRender::GemObj
{
public:
  GemObj( PointF pos, const Texture &tex );
  void Render() const;
  void Update( float deltaTime );
  void MoveTo( PointF destPos, float time );

private:
  PointF m_startPos;
  float m_moveTime;
  float m_totlaTime;
  PointF m_curPos;
  PointF m_destPos;
  const Texture &m_tex;
};

#endif // GameFieldRenderGem_h__