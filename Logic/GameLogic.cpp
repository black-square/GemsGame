#include "stdafx.h"
#include "GameLogic.h"
#include <boost/random/uniform_int_distribution.hpp>


static GameLogic::IEvents g_dummyGameLogicEvents; 

//////////////////////////////////////////////////////////////////////////
typedef boost::random::uniform_int_distribution<> TRngGen;
//////////////////////////////////////////////////////////////////////////

GameLogic::GameLogic( GameField &field ): 
  m_field(field),
  m_rng( static_cast<unsigned int>(std::time(0)) ), 
  m_pEvents(&g_dummyGameLogicEvents)
{

}
//////////////////////////////////////////////////////////////////////////

void GameLogic::SetEventsHandler( IEvents *pEvents /*= 0*/ )
{
  m_pEvents = pEvents != 0 ? pEvents : &g_dummyGameLogicEvents;
  
  for( int y = GameField::FieldSize - 1; y >= 0; --y ) //Reverse order to simplify presentation
    for( int x = 0; x < GameField::FieldSize; ++x )    
      if( m_field.Get(x , y) != GameField::Empty )
        m_pEvents->OnGemAdded( Point(x, y), m_field.Get(x , y) );
}
//////////////////////////////////////////////////////////////////////////

void GameLogic::FillEmptyRandomly()
{
  TRngGen dist( 0, GameField::ColorsCount - 1 );

  for( int y = GameField::FieldSize - 1; y >= 0; --y ) //Reverse order to simplify presentation
    for( int x = 0; x < GameField::FieldSize; ++x )    
      if( m_field.Get(x , y) == GameField::Empty )
      {
        const GameField::Color cl = static_cast<GameField::Color>(dist(m_rng));

        m_field.Set( x, y, cl );
        m_pEvents->OnGemAdded( Point(x, y), cl );
      } 
}
//////////////////////////////////////////////////////////////////////////
 
bool GameLogic::CheckFillEmptyToDown() const
{
  for( int x = 0; x < GameField::FieldSize; ++x )
  {
    int y = 0;

    while( y < GameField::FieldSize && m_field.Get(x, y) == GameField::Empty )
      ++y;

    while( y < GameField::FieldSize && m_field.Get(x, y) != GameField::Empty )
      ++y;

    if( y != GameField::FieldSize )
      return false;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////

void GameLogic::FillEmptyToDown()
{
  for( int x = 0; x < GameField::FieldSize; ++x )
  {
    bool hasMoved;

    do 
    {
      hasMoved = false;

      for( int y = GameField::FieldSize - 1; y > 0; --y )
      {
        const Point upper(x, y - 1);
        const Point cur(x, y);      

        if( m_field.Get(cur) == GameField::Empty && m_field.Get(upper) != GameField::Empty )
        {
          m_field.Set( cur, m_field.Get(upper) );
          m_field.Set( upper, GameField::Empty );
          hasMoved = true;

          m_pEvents->OnGemMove( upper, cur );
        }
      }
    } 
    while (hasMoved);
  }

  ASSERT( CheckFillEmptyToDown() );
}
//////////////////////////////////////////////////////////////////////////

bool GameLogic::FindRemoveFillOnce()
{
  TPoints matches;

  if( !FindMatches(matches) )
    return false;

  Remove( matches );
  FillEmptyToDown();
  FillEmptyRandomly(); 

  return true;
}
//////////////////////////////////////////////////////////////////////////

void GameLogic::FindRemoveFillCompleate()
{
  while( FindRemoveFillOnce() ) 
  {
    //Nothing
  }
}
//////////////////////////////////////////////////////////////////////////

class GameLogic::FieldProxyOrigin
{
public:
  explicit FieldProxyOrigin( GameField &field ): m_field(field) {} 
  
  bool IsValid( int x, int y ) const { return m_field.IsValid( x, y ); }
  GameField::Color Get( int x, int y ) const { return m_field.Get(x, y); }
  void Set( int x, int y, GameField::Color cl ){ m_field.Set(x, y, cl); }
  Point ToRealPoint( int x, int y ) const { return Point(x, y); }
  
  bool IsValid( Point pt ) const { return IsValid( pt.x, pt.y ); }
  GameField::Color Get( Point pt ) const { return Get( pt.x, pt.y ); }
  void Set( Point pt, GameField::Color cl ) { Set( pt.x, pt.y, cl ); }
  Point ToRealPoint( Point pt ) const { return ToRealPoint(pt.x, pt.y); }
  

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
  Point ToRealPoint( int x, int y ) const { return Point(y, x); }

  bool IsValid( Point pt ) const { return IsValid( pt.x, pt.y ); }
  GameField::Color Get( Point pt ) const { return Get( pt.x, pt.y ); }
  void Set( Point pt, GameField::Color cl ) { Set( pt.x, pt.y, cl ); } 
  Point ToRealPoint( Point pt ) const { return ToRealPoint(pt.x, pt.y); }

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
void GameLogic::FindAndAppendMatched( FieldProxyT field, std::vector<Point> &toDestr )
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

bool GameLogic::FindMatches( TPoints &matches ) const
{
  matches.clear();

  FindAndAppendMatched( FieldProxyOrigin(m_field), matches );
  FindAndAppendMatched( FieldProxyMirrored(m_field), matches );                                                      

  return !matches.empty();
}
//////////////////////////////////////////////////////////////////////////

void GameLogic::Remove( const TPoints &matches )
{
  BOOST_FOREACH( const Point &pt, matches )
    if( m_field.Get(pt) != GameField::Empty )
    { 
      m_field.Set( pt, GameField::Empty );
      m_pEvents->OnGemDestroyed( pt );
    }
}
//////////////////////////////////////////////////////////////////////////

template< class FieldProxyT, int N >
GameField::Color GameLogic::IsPatMatched( FieldProxyT field, Point cur, const Point (&pat)[N] )
{
  if( !field.IsValid(cur) )
    return GameField::Empty;

  const GameField::Color clCur = field.Get(cur);

  if( clCur == GameField::Empty )
    return GameField::Empty; 

  BOOST_FOREACH( const Point &pt, pat )
  {
    const Point realPat = cur + pt;

    if( !field.IsValid(realPat) || field.Get(realPat) != clCur )
      return GameField::Empty;
  } 

  return clCur;
}
//////////////////////////////////////////////////////////////////////////

template< class FieldProxyT, int N >
void GameLogic::CheckPossibleMoves( FieldProxyT field, Point cur, GameField::Color cl, const TOneMove (&patterns)[N], TMoves &moves )
{
  ASSERT( cl != GameField::Empty );

  BOOST_FOREACH( const TOneMove &pat, patterns )
  {
    const Point realPat = cur + pat.from;

    if( field.IsValid(realPat) && field.Get(realPat) == cl )
    {
      moves.push_back( std::make_pair(field.ToRealPoint(realPat), field.ToRealPoint(cur + pat.to)) );
    }
  } 
}
//////////////////////////////////////////////////////////////////////////

struct GameLogic::TOneMove
{
  Point from;
  Point to;
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
  static const Point rgPat1[] = 
  { 
    Point(0, +1) 
  };

  static const TOneMove rgPsbl1[] = 
  { 
    { Point(-1, -1), Point(0, -1) }, 
    { Point( 0, -2), Point(0, -1) }, 
    { Point(+1, -1), Point(0, -1) },
    { Point(-1, +2), Point(0, +2) },
    { Point( 0, +3), Point(0, +2) }, 
    { Point(+1, +2), Point(0, +2) }
  };

  // 0 @ 0
  // X 0 X
  // 0 @ 0 
  static const Point rgPat2[] = 
  { 
    Point(0, +2) 
  };

  static const TOneMove rgPsbl2[] = 
  { 
    { Point(-1, +1), Point(0, +1) },
    { Point(+1, +1), Point(0, +1) }
  };

  for( int x = 0; x < GameField::FieldSize; ++x )
    for( int y = 0; y < GameField::FieldSize; ++y )
    {
      const Point cur(x, y);
      
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
//////////////////////////////////////////////////////////////////////////

void GameLogic::FindAllMoves( TMoves &moves ) const
{
  moves.clear();

  FindAllMovesImpl( FieldProxyOrigin(m_field), moves );
  FindAllMovesImpl( FieldProxyMirrored(m_field), moves );                                                      
}
//////////////////////////////////////////////////////////////////////////
const GameLogic::TMove &GameLogic::GetRand( const TMoves &moves ) const
{
  ASSERT( !moves.empty() ); 
  return moves[ TRngGen( 0, moves.size() - 1 )( m_rng ) ];
}
//////////////////////////////////////////////////////////////////////////

bool GameLogic::IsPossibleMove( Point p1, Point p2 )
{
  ASSERT( GameField::IsValid(p1) );
  ASSERT( GameField::IsValid(p2) );
  return manhattanDistance(p1, p2) == 1;
}
//////////////////////////////////////////////////////////////////////////

void GameLogic::SwapImpl( Point p1, Point p2 )
{
  const GameField::Color cl = m_field.Get( p1 );
  m_field.Set( p1, m_field.Get( p2 ) );
  m_field.Set( p2, cl );
}
//////////////////////////////////////////////////////////////////////////

void GameLogic::Swap( Point p1, Point p2 ) 
{
  SwapImpl( p1, p2 );
  m_pEvents->OnGemSwap( p1, p2 );
}
//////////////////////////////////////////////////////////////////////////

bool GameLogic::MakeMove( const TMove &move )
{
  ASSERT( IsPossibleMove( move.first, move.second) );
  SwapImpl( move.first, move.second );
  
  TPoints matches;

  if( !FindMatches(matches) )
  {
    SwapImpl( move.first, move.second );
    return false;
  }

   m_pEvents->OnGemSwap( move.first, move.second );
     
   Remove( matches );
   FillEmptyToDown();
   FillEmptyRandomly(); 

   return true;
}
//////////////////////////////////////////////////////////////////////////

void GameLogic::RecreateField()
{
  for( int x = 0; x < GameField::FieldSize; ++x )
    for ( int y = 0; y < GameField::FieldSize; ++y )
      if( m_field.Get(x, y) != GameField::Empty )
      { 
        m_field.Set( x, y, GameField::Empty );
        m_pEvents->OnGemDestroyed( Point(x, y) );
      }

  FillEmptyRandomly();
}


