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
  explicit GameLogic( GameField &field );

  void FillEmptyRandomly();
  bool FindMatches( TPoints &matches ) const;
  void Remove( const TPoints &matches );

  void FindAllMoves( TMoves &moves ) const;
  void FillEmptyToDown();
  bool FindRemoveFillOnce( TPoints &matches );
  void FindRemoveFillCompleate();

  void Swap( Point p1, Point p2 );
  bool MakeMove( const TMove &move, TPoints &matches );

  static bool IsPossibleMove( Point p1, Point p2 );
  const TMove &GetRand( const TMoves &moves ) const;

  void SetEventsHandler( IEvents *pEvents = 0 ); 
  void RecreateField();

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

  void SwapImpl( Point p1, Point p2 );
  bool CheckFillEmptyToDown() const;

private:
  GameField &m_field;
  mutable boost::random::mt19937 m_rng;
  IEvents *m_pEvents;  
};
//////////////////////////////////////////////////////////////////////////

struct GameLogic::IEvents
{
  virtual ~IEvents() {}

  virtual void OnGemAdded( Point p, GameField::Color cl ) {}
  virtual void OnGemSwap( Point p1, Point p2 ) {}
  virtual void OnGemMove( Point from, Point to ) {}
  virtual void OnGemDestroyed( Point p ) {}
};

#endif // GameLogic_h__
