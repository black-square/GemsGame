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
  
  static bool IsValid( int x, int y )
  {
    return x >= 0 && x < FieldSize && y >= 0 && y < FieldSize;
  }

  static bool IsValid( Point pt )
  {
    return IsValid( pt.x, pt.y );
  }

  static bool IsValid( Color cl )
  {
    return cl >= Empty && cl < ColorsCount;
  }

  Color Get( int x, int y ) const
  {
    ASSERT( IsValid(x, y) );
    return m_field[x][y];
  }
  
  void Set( int x, int y, Color cl )
  {
    ASSERT( IsValid(x, y) );
    ASSERT( IsValid(cl) );
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

  static Size GetSize()
  {
    return Size(FieldSize, FieldSize);
  }

private:
  Color m_field[FieldSize][FieldSize];
};


#endif // GameField_h__
