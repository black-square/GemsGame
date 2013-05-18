#ifndef GameLogic_h__
#define GameLogic_h__

#include "GameField.h"
#include <boost/random/mersenne_twister.hpp>

class GameLogic
{
public:
  typedef std::vector< std::pair<point_t, point_t> > TMoves;

public:
  GameLogic();

  void FillEmptyRandomly( GameField &field ) const;
  static bool DestroyMatched( GameField &field );
  static void FindAllMoves( GameField &field, TMoves &moves );

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
  static boost::optional<point_t> IsOneOfPatMatched( FieldProxyT field, point_t cur, GameField::Color cl, const point_t (&pat)[N] );

private:
  mutable boost::random::mt19937 m_rng;
};
#endif // GameLogic_h__
