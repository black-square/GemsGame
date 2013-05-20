#include "stdafx.h"
#include "AutoPlay.h"

void AutoPlay::Reset()
{
  m_state = FindMatches;
  m_marks.clear();
}
//////////////////////////////////////////////////////////////////////////

void AutoPlay::Update( GameField &field, GameLogic &logic )
{
  switch(m_state)
  {
  case FindMatches:
    if ( logic.FindMatches(field, m_marks) )
      m_state = RemoveMarks;
    else
    {
      GameLogic::TMoves moves;
      logic.FindAllMoves( field, moves );
      m_marks.clear();

      BOOST_FOREACH( const GameLogic::TMoves::value_type &cur, moves )
        m_marks.push_back( cur.first );  

      if( !moves.empty() )
        m_state = FindAllMoves;
    }

    break;
  case RemoveMarks:
    logic.Remove(field, m_marks);
    m_marks.clear();
    m_state = FillEmptyToDown;
    break;
  case FillEmptyToDown:
    logic.FillEmptyToDown( field );
    m_state = FillEmptyRandomly; 
    break;

  case FillEmptyRandomly:
    logic.FillEmptyRandomly( field );
    m_state = FindMatches; 
    break;

  case FindAllMoves:
    {
      GameLogic::TMoves moves;
      logic.FindAllMoves( field, moves );
      ASSERT( !moves.empty() );
      m_nextMove = logic.GetRand( moves );
    }

    m_marks.clear();
    m_marks.push_back( m_nextMove.first );
    m_marks.push_back( m_nextMove.second );

    m_state = DoMove;
    break;

  case DoMove:  
    logic.Swap( field, m_nextMove.first, m_nextMove.second );
    VERIFY( logic.FindMatches(field, m_marks) );
    m_state = RemoveMarks;
    break;
  }
}
