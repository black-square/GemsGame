#ifndef AutoPlay_h__
#define AutoPlay_h__

#include "GameField.h"
#include "GameLogic.h"

class AutoPlay
{
public:
  AutoPlay() {  Reset(); }

  void Reset();
  void Update( GameField &field, GameLogic &logic );
  const GameLogic::TPoints &GetMarks() const { return m_marks; }

private:
  enum State
  {
    FindMatches,
    RemoveMarks,
    FillEmptyToDown,
    FillEmptyRandomly,
    FindAllMoves,
    DoMove
  };

private:
  GameLogic::TPoints m_marks;
  State m_state;
  GameLogic::TMove m_nextMove;
};

#endif // AutoPlay_h__
