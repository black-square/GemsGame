#ifndef GameFieldRender_h__
#define GameFieldRender_h__

#include "GameField.h"
#include "Texture.h"

class GameFieldRender
{
public:
  typedef boost::optional<point_t> PosOpt;

public:
  GameFieldRender( const GameField &field ): m_field(field), m_cellSize(0) {}

  void Init( point_t pos, int cellSize );
  void Render( SDL_Surface* pSurface ) const; 
  void RenderMark( SDL_Surface* pSurface, point_t pt ) const;

  PosOpt GetGemPos( point_t mousePos ) const;

private:
  rect_t GetBoarders() const;

private:
  const GameField &m_field;
  point_t m_pos;
  int m_cellSize;
  Texture m_gems[GameField::ColorsCount];
  Texture m_mark;
};


#endif // GameFieldRender_h__
