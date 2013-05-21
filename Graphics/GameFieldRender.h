#ifndef GameFieldRender_h__
#define GameFieldRender_h__

#include "Logic/GameLogic.h"
#include "Texture.h"

class GameFieldRender: boost::noncopyable, public GameLogic::IEvents
{
public:
  typedef boost::optional<Point> PosOpt;

public:
  GameFieldRender();

  void Init( Point pos, int cellSize );
  void Render() const; 
  void Update( float deltaTime );

  PosOpt GetGemPos( Point mousePos ) const;

private:
  typedef PointBase<float> PointF;
  class GemObj;
  typedef boost::shared_ptr<GemObj> TGemPtr; 

private:
  void OnGemAdded( Point p, GameField::Color cl );
  void OnGemSwap( Point p1, Point p2 );
  void OnGemMove( Point p1, Point p2 );
  void OnGemDestroyed( Point p );

  const TGemPtr &Gem( Point p ) const;
  TGemPtr &Gem( Point p );
 
private:
  Rect GetBoarders() const;
  PointF fieldToScreen( Point p ) const;
  Point screenToField( PointF p ) const;
  static Point Round( PointF p );

private:
  Point m_pos;
  int m_cellSize;
  Texture m_texGems[GameField::ColorsCount];
  TGemPtr m_gems[GameField::FieldSize][GameField::FieldSize];
  mutable boost::random::mt19937 m_rng;
};


#endif // GameFieldRender_h__