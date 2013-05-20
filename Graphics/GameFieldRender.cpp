#include "stdafx.h"
#include "GameFieldRender.h"
#include "GameFieldRenderGemObj.h"

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
Point GameFieldRender::fieldToScreen( Point p ) const
{
  return m_pos + p * m_cellSize;
}
//////////////////////////////////////////////////////////////////////////

Point GameFieldRender::screenToField( Point p ) const
{
  return (p - m_pos) / m_cellSize; 
}
//////////////////////////////////////////////////////////////////////////

GameFieldRender::PosOpt GameFieldRender::GetGemPos( Point mousePos ) const
{
  if( !GetBoarders().isContain(mousePos) )
    return boost::none;

  return (mousePos - m_pos) / m_cellSize;
}
//////////////////////////////////////////////////////////////////////////

const GameFieldRender::TGemPtr &GameFieldRender::Gem( Point p ) const
{
  ASSERT( p.x >= 0 && p.x < GameField::FieldSize );
  ASSERT( p.y >= 0 && p.y < GameField::FieldSize );

  return m_gems[p.x][p.y];
}
//////////////////////////////////////////////////////////////////////////

GameFieldRender::TGemPtr &GameFieldRender::Gem( Point p )
{
  ASSERT( p.x >= 0 && p.x < GameField::FieldSize );
  ASSERT( p.y >= 0 && p.y < GameField::FieldSize );

  return m_gems[p.x][p.y];
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::OnGemAdded( Point p, GameField::Color cl )
{
  ASSERT( !Gem(p) );

  ASSERT(cl >= 0 && cl < ARRAY_SIZE(m_texGems) );

  TGemPtr pGem = boost::make_shared<GemObj>( fieldToScreen(Point(p.x, p.y - GameField::FieldSize)), m_texGems[cl] );
  Gem(p) = pGem;
  pGem->MoveTo( fieldToScreen(p), 1 );
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

