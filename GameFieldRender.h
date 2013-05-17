#ifndef GameFieldRender_h__
#define GameFieldRender_h__

#include "GameField.h"
#include "Texture.h"

class GameFieldRender
{
public:
  GameFieldRender( const GameField &field ): m_field(field) {}

  void Init();
  void Render( SDL_Surface* pSurface, point_t pos ); 

private:
  const GameField &m_field;
  Texture m_gems[GameField::ColorsCount];
};


#endif // GameFieldRender_h__
