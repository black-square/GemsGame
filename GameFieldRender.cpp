#include "stdafx.h"
#include "GameFieldRender.h"

void GameFieldRender::Init( point_t pos, int cellSize )
{
  m_pos = pos;
  m_cellSize = cellSize;

  for( int i = 0; i < GameField::ColorsCount; ++i )
    m_gems[i].Load( MakeString(FMT("./_data/gem_%02d.png") % (i + 1)).c_str() );  
}
//////////////////////////////////////////////////////////////////////////

void GameFieldRender::Render( SDL_Surface *pSurface ) const
{
  for( int x = 0; x < GameField::FieldSize; ++x )
    for( int y = 0; y < GameField::FieldSize; ++y )
    {
      const GameField::Color cl = m_field.Get( x, y );

      if( cl != GameField::Empty )
      {
        ASSERT(cl >= 0 && cl < ARRAY_SIZE(m_gems) );
        Draw( pSurface, m_gems[cl], m_pos + point_t( x * m_cellSize, y * m_cellSize ) );
      }
    }
}
//////////////////////////////////////////////////////////////////////////

rect_t GameFieldRender::GetBoarders() const
{
  return rect_t( m_pos, m_pos + point_t(m_cellSize * GameField::FieldSize, m_cellSize * GameField::FieldSize ) );
}
//////////////////////////////////////////////////////////////////////////

GameFieldRender::PosOpt GameFieldRender::GetGemPos( point_t mousePos ) const
{
  if( !GetBoarders().isContain(mousePos) )
    return boost::none;

  return (mousePos - m_pos) / m_cellSize;
}

