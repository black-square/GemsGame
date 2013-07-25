#include "stdafx.h"
#include "GameFieldRender.h"
#include "GameFieldRenderGemObj.h"
#include <boost/random/uniform_real_distribution.hpp>

//////////////////////////////////////////////////////////////////////////

GameFieldRender::GameFieldRender() : 
  m_cellSize(0), m_rng( static_cast<unsigned int>(std::time(0)) )
{

}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::Init( Point pos, int cellSize )
{
  m_pos = pos;
  m_cellSize = cellSize;

  for( int i = 0; i < GameField::ColorsCount; ++i )
    m_texGems[i].Load( MakeString(FMT("./_data/gem_%02d.png") % (i + 1)).c_str() ); 
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::Render() const
{
  for( int y = GameField::FieldSize - 1; y >= 0; --y )
    for( int x = 0; x < GameField::FieldSize; ++x )
    {
      const TGemPtr &pGem = Gem( Point(x, y) );

      if( pGem )
        pGem->Render();
    }
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::Update( float deltaTime )
{
  for( int y = GameField::FieldSize - 1; y >= 0; --y )
    for( int x = 0; x < GameField::FieldSize; ++x )
    {
      const TGemPtr &pGem = Gem( Point(x, y) );

      if( pGem )
        pGem->Update( deltaTime );
    }
}

Rect GameFieldRender::GetBoarders() const
{
  return Rect( m_pos, Size(m_cellSize, m_cellSize) * GameField::FieldSize );
}
//////////////////////////////////////////////////////////////////////////
GameFieldRender::PointF GameFieldRender::fieldToScreen( Point p ) const
{
  return PointF( m_pos + p * m_cellSize );
}
//////////////////////////////////////////////////////////////////////////

Point GameFieldRender::screenToField( PointF p ) const
{
  return (Round(p) - m_pos) / m_cellSize; 
}
//////////////////////////////////////////////////////////////////////////

Point GameFieldRender::screenToField( Point p ) const
{
  return (p - m_pos) / m_cellSize;
}
//////////////////////////////////////////////////////////////////////////

Point GameFieldRender::Round( PointF p )
{
  return Point( p + PointF(0.5f, 0.5f) );
}
//////////////////////////////////////////////////////////////////////////

const GameFieldRender::TGemPtr &GameFieldRender::Gem( Point p ) const
{
  ASSERT( IsValid(p) );
  return m_gems[p.x][p.y];
}
//////////////////////////////////////////////////////////////////////////

GameFieldRender::TGemPtr &GameFieldRender::Gem( Point p )
{
  ASSERT( IsValid(p) );
  return m_gems[p.x][p.y];
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::OnGemAdded( Point p, GameField::Color cl )
{
  ASSERT( !Gem(p) );
  ASSERT( GameField::IsValid(cl) );

  PointF startPos( fieldToScreen(Point(p.x, -1)) );
                                              
  const float maxPos = GameField::FieldSize - 1.0f;
  const float posYFactor = maxPos - p.y;
  const float posXFactor = 1 - p.x / maxPos;
  const float posFactor = posYFactor + 3.f * posXFactor;

  startPos.y -= 50 * posFactor; 

  TGemPtr pGem = boost::make_shared<GemObj>( startPos, m_texGems[cl] );
  Gem(p) = pGem;

  pGem->FallTo( fieldToScreen(p), 1 + 0.06f * posFactor );
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::OnGemSwap( Point p1, Point p2 )
{

}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::OnGemMove( Point p1, Point p2 )
{

}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::OnGemDestroyed( Point p )
{

}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::LButtonDown( Point pos )
{
  if( !GetBoarders().isContain(pos) )
    return;

  const Point posField = screenToField(pos);

  if( const TGemPtr &p = Gem( posField ) )
  {
    m_pGemDragged = p;
    m_draggedPos = posField;
  }
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::LButtonUp( Point pos )
{
  if( !m_pGemDragged.expired() )
  {
    BringNeighborsBack( m_draggedPos );
    m_pGemDragged.reset();
  }
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::BringNeighborsBack( Point p )
{
  static const Point rgNeighbors[] = { Point(0, 0), Point(-1, 0), Point(0, -1), Point(+1, 0), Point(0, +1) };

  BOOST_FOREACH( const Point &pt, rgNeighbors )
  {
    const Point pos = p + pt;

    if( IsValid(pos) )
      if( const TGemPtr &pPair = Gem( pos ) )
        pPair->UpdateDragPoint( fieldToScreen(pos) );
  }  
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::MouseMove( Point pos )
{
  if( !GetBoarders().isContain(pos) )
    return;

  const TGemPtr &p = m_pGemDragged.lock();
  const Point posField = screenToField(pos);
  const bool canMove = m_draggedPos == posField || GameLogic::IsPossibleMove(m_draggedPos, posField);

  if( p && canMove )
  {
    BringNeighborsBack( m_draggedPos );
    
    p->UpdateDragPoint( fieldToScreen(posField) );  
    
    if( const TGemPtr &pPair = Gem( posField ) )
      pPair->UpdateDragPoint( fieldToScreen(m_draggedPos) );       
  }
}

