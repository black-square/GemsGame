#ifndef GameFieldRender_h__
#define GameFieldRender_h__

#include "Logic/GameLogic.h"
#include "Texture.h"

//////////////////////////////////////////////////////////////////////////

class GameFieldRender: boost::noncopyable, public GameLogic::IEvents
{
public:
  GameFieldRender();
  ~GameFieldRender();

  void Init( Point pos, int cellSize );
  void Render() const; 
  void RenderMark( Point pt ) const;
  void Update( float deltaTime );

  void LButtonDown( Point pos );
  void LButtonUp( Point pos, GameLogic::TMove &move );
  void MouseMove( Point pos );

private:
  typedef PointBase<float> PointF;
  class GemObj;
  struct IGemState;
  class DefaultState;
  class FallState;
  class SpringState;
  struct FallParams;
  class FallingGemsManager;
  typedef boost::shared_ptr<GemObj> TGemPtr;
  typedef boost::weak_ptr<GemObj> TGemWeakPtr;

private:  
  void OnGemAdded( Point p, GameField::Color cl );
  void OnGemSwap( Point p1, Point p2 );
  void OnGemMove( Point from, Point to );
  void OnGemDestroyed( Point p );

  static bool IsValid( Point p ) { return GameField::IsValid(p); }
  const TGemPtr &Gem( Point p ) const;
  TGemPtr &Gem( Point p );
 
private:
  Rect GetBoarders() const;
  PointF fieldToScreen( Point p ) const;
  Point screenToField( PointF p ) const;
  Point screenToField( Point p ) const;
  static Point Round( PointF p );
  void BringNeighborsBack( Point p );
  Point ClampToSuitableMove( Point from, Point to ) const;

private:
  Point m_pos;
  int m_cellSize;
  Texture m_texGems[GameField::ColorsCount];
  Texture m_texMark;
  TGemPtr m_gems[GameField::FieldSize][GameField::FieldSize];
  TGemWeakPtr m_pGemDragged;
  Point m_draggedPos;
  const boost::scoped_ptr<FallingGemsManager> m_pFallingGemsManager;
};


#endif // GameFieldRender_h__