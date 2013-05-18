#ifndef GameLogic_h__
#define GameLogic_h__

#include "GameField.h"
#include <boost/random/mersenne_twister.hpp>

class GameLogic
{
public:
  typedef std::vector< std::pair<point_t, point_t> > TMoves;
  typedef std::vector< point_t > TPoints;

public:
  GameLogic();

  void FillEmptyRandomly( GameField &field ) const;
  static bool FindMatches( GameField &field, TPoints &matches );
  static void Remove( GameField &field, const TPoints &matches );
  static bool RemoveMatches( GameField &field );

  static void FindAllMoves( GameField &field, TMoves &moves );
  static void FillEmptyToDown( GameField &field );
  static bool DestroyAndFillEmptyToDown( GameField &field );

private:
  class FieldProxyOrigin;
  class FieldProxyMirrored;
  class ThreeInLineFnct;

private:
  template< class FieldProxyT >
  static void FindAndAppendMatched( FieldProxyT field, std::vector<point_t> &toDestr );

  template< class FieldProxyT >
  static void FindAllMovesImpl( FieldProxyT field, TMoves &moves );

  template< class FieldProxyT, int N >
  static GameField::Color IsPatMatched( FieldProxyT field, point_t cur, const point_t (&pat)[N] );

  template< class FieldProxyT, int N >
  static void CheckPossibleMoves( FieldProxyT field, point_t cur, GameField::Color cl, const point_t (&pat)[N], TMoves &moves );

private:
  mutable boost::random::mt19937 m_rng;
};
#endif // GameLogic_h__
