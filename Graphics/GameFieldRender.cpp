#include "stdafx.h"
#include "GameFieldRender.h"
#include "GameFieldRenderGemObj.h"
#include <boost/random/uniform_real_distribution.hpp>

//////////////////////////////////////////////////////////////////////////

GameFieldRender::GameFieldRender() : 
  m_cellSize(0), m_rng( static_cast<unsigned int>(std::time(0)) )
{
  m_gemFallingDelay = 0;
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
  
  m_gemFallingDelay = 0;
}
//////////////////////////////////////////////////////////////////////////

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
                                             
  TGemPtr pGem = boost::make_shared<GemObj>( startPos, m_texGems[cl] );
  Gem(p) = pGem;

  const float fieldSize = GameField::FieldSize - 1.f;
  const float firstCellTime = 0.2f;
  const float accel = FallStateAccel::CalcAccel( float(m_cellSize), firstCellTime );

  pGem->FallTo( fieldToScreen(p), accel,  firstCellTime * (fieldSize - p.y) + (fieldSize - p.x) * 0.06f );
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::OnGemMove( Point from, Point to )
{
  TGemPtr &pGem1 = Gem(from);
  TGemPtr &pGem2 = Gem(to);
  
  ASSERT( pGem1 );
  ASSERT( !pGem2 );

  pGem2 = pGem1;
  pGem1.reset();

  const float dist = fieldToScreen(to - from).y;
  
  //pGem2->FallTo( fieldToScreen(p2), std::sqrt(dist/300.f) );
  pGem2->SetPos( fieldToScreen(to) );
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::OnGemDestroyed( Point p )
{
  TGemPtr &pGem = Gem(p);

  ASSERT( pGem );
  pGem.reset();
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::OnGemSwap( Point p1, Point p2 )
{
  TGemPtr &pGem1 = Gem(p1);
  TGemPtr &pGem2 = Gem(p2);

  std::swap( pGem1, pGem2 );
  pGem1->UpdateDragPoint( fieldToScreen(p1) );
  pGem2->UpdateDragPoint( fieldToScreen(p2) );
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

Point GameFieldRender::ClampToSuitableMove( Point from, Point to ) const
{ 
  const Point diff = abs(to - from);

  if( diff.x > diff.y )
    to.y = from.y;
  else if( diff.y > diff.x )
    to.x = from.x;

  const Rect moveRect( from - Point(1, 1), Size(3, 3) );
  const Rect fieldRect( Point(), GameField::GetSize() );

  return clamp( clamp( to, moveRect ), fieldRect );
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

void GameFieldRender::LButtonUp( Point pos, GameLogic::TMove &move )
{
  move = GameLogic::TMove();

  if( !m_pGemDragged.expired() )
  {
    BringNeighborsBack( m_draggedPos );

    const Point posField = ClampToSuitableMove( m_draggedPos, screenToField(pos) );

    if( GameLogic::IsPossibleMove(m_draggedPos, posField) )
      move = std::make_pair( m_draggedPos, posField );

    m_pGemDragged.reset();
  }
}
//////////////////////////////////////////////////////////////////////////


void GameFieldRender::MouseMove( Point pos )
{
  const TGemPtr &p = m_pGemDragged.lock();

  if( !p )
    return;

  const Point posField = ClampToSuitableMove( m_draggedPos, screenToField(pos) );
  const bool canMove = m_draggedPos == posField || GameLogic::IsPossibleMove(m_draggedPos, posField);

  if( canMove )
  {
    BringNeighborsBack( m_draggedPos );  
    p->UpdateDragPoint( fieldToScreen(posField) );  
    
    if( const TGemPtr &pPair = Gem( posField ) )
      pPair->UpdateDragPoint( fieldToScreen(m_draggedPos) );       
  }
}

