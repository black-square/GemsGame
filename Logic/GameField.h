#ifndef GameField_h__
#define GameField_h__

class GameField
{
public:
  enum { FieldSize = 8 };
  
  enum Color
  {
    Empty = -1,
    ColorsCount = 5
  };

public:
  GameField();

  void Clear();
  
  bool IsValid( int x, int y ) const
  {
    return x >= 0 && x < FieldSize && y >= 0 && y < FieldSize;
  }

  bool IsValid( Point pt ) const
  {
    return IsValid( pt.x, pt.y );
  }

  Color Get( int x, int y ) const
  {
    ASSERT( x >= 0 && x < FieldSize && y >= 0 && y < FieldSize );
    return m_field[x][y];
  }
  
  void Set( int x, int y, Color cl )
  {
    ASSERT( x >= 0 && x < FieldSize && y >= 0 && y < FieldSize );
    ASSERT( cl >= Empty && cl < ColorsCount );
    m_field[x][y] = cl;
  }
  
  Color Get( Point pt ) const 
  { 
    return Get( pt.x, pt.y ); 
  }
  
  void Set( Point pt, Color cl ) 
  { 
    Set( pt.x, pt.y, cl ); 
  } 

private:
  Color m_field[FieldSize][FieldSize];
};


#endif // GameField_h__
