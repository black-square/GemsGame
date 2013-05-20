#ifndef GameFieldRender_h__
#define GameFieldRender_h__

#include "Logic/GameField.h"
#include "Texture.h"

class GameFieldRender: boost::noncopyable
{
public:
  typedef boost::optional<Point> PosOpt;

public:
  GameFieldRender( const GameField &field ): m_field(field), m_cellSize(0) {}

  void Init( Point pos, int cellSize );
  void Render() const; 
  void RenderMark( Point pt ) const;

  PosOpt GetGemPos( Point mousePos ) const;

private:
  Rect GetBoarders() const;

private:
  const GameField &m_field;
  Point m_pos;
  int m_cellSize;
  Texture m_gems[GameField::ColorsCount];
  Texture m_mark;
};


#endif // GameFieldRender_h__
