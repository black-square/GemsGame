#include "stdafx.h"
#include "GameFieldRender.h"

void GameFieldRender::Init()
{
  for( int i = 0; i < GameField::ColorsCount; ++i )
    m_gems[i].Load( MakeString(FMT("./_data/gem_%02d.png") % (i + 1)).c_str() );  
}

void GameFieldRender::Render( SDL_Surface *pSurface, point_t pos )
{
  for( int x = 0; x < GameField::FieldSize; ++x )
    for( int y = 0; y < GameField::FieldSize; ++y )
    {
      const GameField::Color cl = m_field.Get( x, y );

      if( cl != GameField::Empty )
      {
        ASSERT(cl >= 0 && cl < ARRAY_SIZE(m_gems) );
        Draw( pSurface, m_gems[cl], pos + point_t( x * 40, y * 40 ) );
      }
    }
}
