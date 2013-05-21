#ifndef GameLogic_h__
#define GameLogic_h__

#include "GameField.h"
#include <boost/random/mersenne_twister.hpp>
    

//////////////////////////////////////////////////////////////////////////

// No moves field: http://gyazo.com/1e67cc33fe5f2e7c4689984e7a4d3bfe
class GameLogic
{
public:
  typedef std::pair<Point, Point> TMove;
  typedef std::vector< TMove > TMoves;
  typedef std::vector< Point > TPoints;
  struct IEvents;

public:
  GameLogic();

  void FillEmptyRandomly( GameField &field ) const;
  static bool FindMatches( GameField &field, TPoints &matches );
  void Remove( GameField &field, const TPoints &matches ) const;
  bool RemoveMatches( GameField &field ) const;

  static void FindAllMoves( GameField &field, TMoves &moves );
  void FillEmptyToDown( GameField &field ) const;
  bool DestroyAndFillEmptyToDown( GameField &field ) const;

  void Swap( GameField &field, Point p1, Point p2 ) const;
  static bool IsPossibleMove( Point p1, Point p2 );
  const TMove &GetRand( const TMoves &moves ) const;

  void SetEventsHandler( IEvents *pEvents = 0 ); 

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
  IEvents *m_pEvents;
};
//////////////////////////////////////////////////////////////////////////

struct GameLogic::IEvents
{
  virtual ~IEvents() {}

  virtual void OnGemAdded( Point p, GameField::Color cl ) {}
  virtual void OnGemSwap( Point p1, Point p2 ) {}
  virtual void OnGemMove( Point p1, Point p2 ) {}
  virtual void OnGemDestroyed( Point p ) {}
};

#endif // GameLogic_h__
