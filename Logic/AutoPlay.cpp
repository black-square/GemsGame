#include "stdafx.h"
#include "AutoPlay.h"

void AutoPlay::Reset()
{
  m_state = FindMatches;
  m_marks.clear();
}
//////////////////////////////////////////////////////////////////////////

void AutoPlay::Update( GameLogic &logic )
{
  switch(m_state)
  {
  case FindMatches:
    if ( logic.FindMatches(m_marks) )
      m_state = RemoveMarks;
    else
    {
      GameLogic::TMoves moves;
      logic.FindAllMoves( moves );
      m_marks.clear();

      BOOST_FOREACH( const GameLogic::TMoves::value_type &cur, moves )
        m_marks.push_back( cur.first );  

      if( !moves.empty() )
        m_state = FindAllMoves;
    }

    break;
  case RemoveMarks:
    logic.Remove(m_marks);
    m_marks.clear();
    m_state = FillEmptyToDown;
    break;
  case FillEmptyToDown:
    logic.FillEmptyToDown();
    m_state = FillEmptyRandomly; 
    break;

  case FillEmptyRandomly:
    logic.FillEmptyRandomly();
    m_state = FindMatches; 
    break;

  case FindAllMoves:
    {
      GameLogic::TMoves moves;
      logic.FindAllMoves( moves );
      ASSERT( !moves.empty() );
      m_nextMove = logic.GetRand( moves );
    }

    m_marks.clear();
    m_marks.push_back( m_nextMove.first );
    m_marks.push_back( m_nextMove.second );

    m_state = DoMove;
    break;

  case DoMove:  
    logic.Swap( m_nextMove.first, m_nextMove.second );
    VERIFY( logic.FindMatches(m_marks) );
    m_state = RemoveMarks;
    break;
  }
}
