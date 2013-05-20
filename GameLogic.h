#ifndef GameLogic_h__
#define GameLogic_h__

#include "GameField.h"
#include <boost/random/mersenne_twister.hpp>
    
struct IGameLogicEvents
{
  virtual void OnSwap( Point p1, Point p2 ) = 0;
};

// No moves field: http://gyazo.com/1e67cc33fe5f2e7c4689984e7a4d3bfe
class GameLogic
{
public:
  typedef std::pair<Point, Point> TMove;
  typedef std::vector< TMove > TMoves;
  typedef std::vector< Point > TPoints;

public:
  GameLogic();

  void FillEmptyRandomly( GameField &field ) const;
  static bool FindMatches( GameField &field, TPoints &matches );
  static void Remove( GameField &field, const TPoints &matches );
  static bool RemoveMatches( GameField &field );

  static void FindAllMoves( GameField &field, TMoves &moves );
  static void FillEmptyToDown( GameField &field );
  static bool DestroyAndFillEmptyToDown( GameField &field );

  void Swap( GameField &field, Point p1, Point p2 ) const;
  int GetRand( int from, int to ) const;

  void SetEventsHandler( IGameLogicEvents *pEvents = 0 ); 

private:
  class FieldProxyOrigin;
  class FieldProxyMirrored;
  class ThreeInLineFnct;
  struct TOneMove;

private:
  template< class FieldProxyT >
  static void FindAndAppendMatched( FieldProxyT field, std::vector<Point> &toDestr );

  template< class FieldProxyT >
  static void FindAllMovesImpl( FieldProxyT field, TMoves &moves );

  template< class FieldProxyT, int N >
  static GameField::Color IsPatMatched( FieldProxyT field, Point cur, const Point (&pat)[N] );

  template< class FieldProxyT, int N >
  static void CheckPossibleMoves( FieldProxyT field, Point cur, GameField::Color cl, const TOneMove (&pat)[N], TMoves &moves );

private:
  mutable boost::random::mt19937 m_rng;
  IGameLogicEvents *m_pEvents;
};
#endif // GameLogic_h__
