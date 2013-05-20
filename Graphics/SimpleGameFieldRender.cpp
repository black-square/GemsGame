#include "stdafx.h"
#include "SimpleGameFieldRender.h"

void SimpleGameFieldRender::Init( Point pos, int cellSize )
{
  m_pos = pos;
  m_cellSize = cellSize;

  for( int i = 0; i < GameField::ColorsCount; ++i )
    m_gems[i].Load( MakeString(FMT("./_data/gem_%02d.png") % (i + 1)).c_str() );
    
  m_mark.Load("./_data/mark2.png");  
}
//////////////////////////////////////////////////////////////////////////

void SimpleGameFieldRender::Render() const
{
  for( int x = 0; x < GameField::FieldSize; ++x )
    for( int y = 0; y < GameField::FieldSize; ++y )
    {
      const GameField::Color cl = m_field.Get( x, y );

      if( cl != GameField::Empty )
      {
        ASSERT(cl >= 0 && cl < ARRAY_SIZE(m_gems) );
        Draw( m_gems[cl], m_pos + Point( x, y ) * m_cellSize );
      }
    }
}
//////////////////////////////////////////////////////////////////////////

void SimpleGameFieldRender::RenderMark( Point pt ) const
{
  Draw( m_mark, m_pos + pt * m_cellSize );
}

Rect SimpleGameFieldRender::GetBoarders() const
{
  return Rect( m_pos, Size(m_cellSize, m_cellSize) * GameField::FieldSize );
}
//////////////////////////////////////////////////////////////////////////

SimpleGameFieldRender::PosOpt SimpleGameFieldRender::GetGemPos( Point mousePos ) const
{
  if( !GetBoarders().isContain(mousePos) )
    return boost::none;

  return (mousePos - m_pos) / m_cellSize;
}

