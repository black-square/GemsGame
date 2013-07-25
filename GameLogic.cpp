#include "stdafx.h"
#include "GameLogic.h"
#include <boost/random/uniform_int_distribution.hpp>


struct DummyGameLogicEvents: public IGameLogicEvents
{
  void OnSwap( point_t p1, point_t p2 ) {}
};

static DummyGameLogicEvents g_dummyGameLogicEvents; 

//////////////////////////////////////////////////////////////////////////
typedef boost::random::uniform_int_distribution<> TRngGen;
//////////////////////////////////////////////////////////////////////////

GameLogic::GameLogic(): 
  m_rng( static_cast<unsigned int>(std::time(0)) ), 
  m_pEvents(&g_dummyGameLogicEvents)
{

}
//////////////////////////////////////////////////////////////////////////

void GameLogic::SetEventsHandler( IGameLogicEvents *pEvents /*= 0*/ )
{
  m_pEvents = pEvents != 0 ? pEvents : &g_dummyGameLogicEvents; 
}
//////////////////////////////////////////////////////////////////////////

void GameLogic::FillEmptyRandomly( GameField &field ) const
{
  TRngGen dist( 0, GameField::ColorsCount - 1 );

  for( int x = 0; x < GameField::FieldSize; ++x )
    for( int y = 0; y < GameField::FieldSize; ++y )
      if( field.Get(x , y) == GameField::Empty )
        field.Set( x , y, static_cast<GameField::Color>(dist(m_rng)) );  
}
//////////////////////////////////////////////////////////////////////////
 
void GameLogic::FillEmptyToDown( GameField &field )
{
  for( int x = 0; x < GameField::FieldSize; ++x )
  {
    bool hasMoved;

    do 
    {
      hasMoved = false;

      for( int y = GameField::FieldSize - 1; y > 0; --y )
      {
        const point_t cur(x, y);
        const point_t next(x, y - 1);
        
        if( field.Get(cur) == GameField::Empty && field.Get(next) != GameField::Empty )
        {
          field.Set( cur, field.Get(next) );
          field.Set( next, GameField::Empty );
          hasMoved = true;
        }
      }
    } 
    while (hasMoved);
  }
}
//////////////////////////////////////////////////////////////////////////

bool GameLogic::DestroyAndFillEmptyToDown( GameField &field )
{
  if( !RemoveMatches(field) )
    return false;

  do 
  {
    FillEmptyToDown( field );  
  } 
  while( RemoveMatches(field) );
  
  return true;
}
//////////////////////////////////////////////////////////////////////////

class GameLogic::FieldProxyOrigin
{
public:
  explicit FieldProxyOrigin( GameField &field ): m_field(field) {} 
  
  bool IsValid( int x, int y ) const { return m_field.IsValid( x, y ); }
  GameField::Color Get( int x, int y ) const { return m_field.Get(x, y); }
  void Set( int x, int y, GameField::Color cl ){ m_field.Set(x, y, cl); }
  point_t ToRealPoint( int x, int y ) const { return point_t(x, y); }
  
  bool IsValid( point_t pt ) const { return IsValid( pt.x, pt.y ); }
  GameField::Color Get( point_t pt ) const { return Get( pt.x, pt.y ); }
  void Set( point_t pt, GameField::Color cl ) { Set( pt.x, pt.y, cl ); }
  point_t ToRealPoint( point_t pt ) const { return ToRealPoint(pt.x, pt.y); }
  

private:
  GameField &m_field;
};
//////////////////////////////////////////////////////////////////////////

class GameLogic::FieldProxyMirrored
{
public:
  explicit FieldProxyMirrored( GameField &field ): m_field(field) {} 

  bool IsValid( int x, int y ) const { return m_field.IsValid( y, x ); }
  GameField::Color Get( int x, int y ) const { return m_field.Get(y, x); }
  void Set( int x, int y, GameField::Color cl ){ m_field.Set(y, x, cl); }
  point_t ToRealPoint( int x, int y ) const { return point_t(y, x); }

  bool IsValid( point_t pt ) const { return IsValid( pt.x, pt.y ); }
  GameField::Color Get( point_t pt ) const { return Get( pt.x, pt.y ); }
  void Set( point_t pt, GameField::Color cl ) { Set( pt.x, pt.y, cl ); } 
  point_t ToRealPoint( point_t pt ) const { return ToRealPoint(pt.x, pt.y); }

private:
  GameField &m_field;
};
//////////////////////////////////////////////////////////////////////////

class GameLogic::ThreeInLineFnct
{
public:
  ThreeInLineFnct(): 
    m_prevCl(GameField::Empty), m_curMatch(1)
  {}

  int operator()( GameField::Color cl )
  {
    if( cl == GameField::Empty || m_prevCl != cl )
    {
      m_prevCl = cl;
      m_curMatch = 1;   
      return 0;
    }

    ++m_curMatch;

    if( m_curMatch < MinMatched )
      return 0;
    else if( m_curMatch == MinMatched )
      return MinMatched;
    else
      return 1;
  }
   
private:
  GameField::Color m_prevCl;
  int m_curMatch;
  enum { MinMatched = 3 };
};
//////////////////////////////////////////////////////////////////////////

template< class FieldProxyT >
void GameLogic::FindAndAppendMatched( FieldProxyT field, std::vector<point_t> &toDestr )
{
  for( int x = 0; x < GameField::FieldSize; ++x )
  {
    ThreeInLineFnct fnct;

    for( int y = 0; y < GameField::FieldSize; ++y )
    {
      int matched = fnct( field.Get(x, y) );  

      while( matched > 0 )
        toDestr.push_back( field.ToRealPoint(x, y - --matched) ); 
    }
  }
}
//////////////////////////////////////////////////////////////////////////

bool GameLogic::FindMatches( GameField &field, TPoints &matches )
{
  matches.clear();

  FindAndAppendMatched( FieldProxyOrigin(field), matches );
  FindAndAppendMatched( FieldProxyMirrored(field), matches );                                                      

  return !matches.empty();
}
//////////////////////////////////////////////////////////////////////////

void GameLogic::Remove( GameField &field, const TPoints &matches )
{
  BOOST_FOREACH( const point_t &pt, matches )
    field.Set( pt, GameField::Empty );
}
//////////////////////////////////////////////////////////////////////////

bool GameLogic::RemoveMatches( GameField &field )
{
  TPoints matches;
  
  FindMatches(field, matches);
  Remove( field, matches );
  return !matches.empty();
}

template< class FieldProxyT, int N >
GameField::Color GameLogic::IsPatMatched( FieldProxyT field, point_t cur, const point_t (&pat)[N] )
{
  if( !field.IsValid(cur) )
    return GameField::Empty;

  const GameField::Color clCur = field.Get(cur);

  if( clCur == GameField::Empty )
    return GameField::Empty; 

  BOOST_FOREACH( const point_t &pt, pat )
  {
    const point_t realPat = cur + pt;

    if( !field.IsValid(realPat) || field.Get(realPat) != clCur )
      return GameField::Empty;
  } 

  return clCur;
}
//////////////////////////////////////////////////////////////////////////

template< class FieldProxyT, int N >
void GameLogic::CheckPossibleMoves( FieldProxyT field, point_t cur, GameField::Color cl, const TOneMove (&patterns)[N], TMoves &moves )
{
  ASSERT( cl != GameField::Empty );

  BOOST_FOREACH( const TOneMove &pat, patterns )
  {
    const point_t realPat = cur + pat.from;

    if( field.IsValid(realPat) && field.Get(realPat) == cl )
    {
      moves.push_back( std::make_pair(field.ToRealPoint(realPat), field.ToRealPoint(cur + pat.to)) );
    }
  } 
}
//////////////////////////////////////////////////////////////////////////

struct GameLogic::TOneMove
{
  point_t from;
  point_t to;
};
//////////////////////////////////////////////////////////////////////////

template< class FieldProxyT >
void GameLogic::FindAllMovesImpl( FieldProxyT field, TMoves &moves )
{  
  // 0 X 0
  // X 0 X
  // 0 @ 0 
  // 0 @ 0
  // X 0 X
  // 0 X 0
  static const point_t rgPat1[] = 
  { 
    point_t(0, +1) 
  };

  static const TOneMove rgPsbl1[] = 
  { 
    { point_t(-1, -1), point_t(0, -1) }, 
    { point_t( 0, -2), point_t(0, -1) }, 
    { point_t(+1, -1), point_t(0, -1) },
    { point_t(-1, +2), point_t(0, +2) },
    { point_t( 0, +3), point_t(0, +2) }, 
    { point_t(+1, +2), point_t(0, +2) }
  };

  // 0 @ 0
  // X 0 X
  // 0 @ 0 
  static const point_t rgPat2[] = 
  { 
    point_t(0, +2) 
  };

  static const TOneMove rgPsbl2[] = 
  { 
    { point_t(-1, +1), point_t(0, +1) },
    { point_t(+1, +1), point_t(0, +1) }
  };

  for( int x = 0; x < GameField::FieldSize; ++x )
    for( int y = 0; y < GameField::FieldSize; ++y )
    {
      const point_t cur(x, y);
      
      {
        const GameField::Color cl = IsPatMatched( field, cur, rgPat1 );
        
        if( cl != GameField::Empty )
          CheckPossibleMoves( field, cur, cl, rgPsbl1, moves );
      }
      
      {
        const GameField::Color cl = IsPatMatched(field, cur, rgPat2 );

        if( cl != GameField::Empty )
          CheckPossibleMoves( field, cur, cl, rgPsbl2, moves );
      }
    }
}


void GameLogic::FindAllMoves( GameField &field, TMoves &moves )
{
  moves.clear();

  FindAllMovesImpl( FieldProxyOrigin(field), moves );
  FindAllMovesImpl( FieldProxyMirrored(field), moves );                                                      
}
//////////////////////////////////////////////////////////////////////////

int GameLogic::GetRand( int from, int to ) const
{
  return TRngGen( from, to )( m_rng );
}
//////////////////////////////////////////////////////////////////////////

void GameLogic::Swap( GameField &field, point_t p1, point_t p2 ) const 
{
  const GameField::Color cl = field.Get( p1 );
  field.Set( p1, field.Get( p2 ) );
  field.Set( p2, cl );

  m_pEvents->OnSwap( p1, p2 );
}


