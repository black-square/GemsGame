#include "stdafx.h"
#include "GameField.h"

GameField::GameField()
{
  for( int i = 0; i < FieldSize; ++i )
    for( int j = 0; j < FieldSize; ++j )
      m_field[i][j] = Empty; 
}
