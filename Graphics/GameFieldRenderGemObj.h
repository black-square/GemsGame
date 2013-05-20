#ifndef GameFieldRenderGem_h__
#define GameFieldRenderGem_h__

#include "GameFieldRender.h"

class GameFieldRender::GemObj
{
public:
  GemObj( Point pos, const Texture &tex );
  void Render() const;
  void Update( float deltaTime );
  void MoveTo( Point destPos, float time );

private:
  PointBase<float> m_startPos;
  float m_moveTime;
  float m_totlaTime;
  PointBase<float> m_curPos;
  PointBase<float> m_destPos;
  const Texture &m_tex;
};

#endif // GameFieldRenderGem_h__