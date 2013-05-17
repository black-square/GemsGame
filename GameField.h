#ifndef GameField_h__
#define GameField_h__

class GameField
{
public:
  enum { FieldSize = 8 };
  
  enum Color
  {
    Empty = -1,
    Color_01,
    Color_02,
    Color_03,
    Color_04,
    Color_05,
    ColorsCount
  };

public:
  GameField();
  
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
  
  Color Get( point_t pt ) const 
  { 
    return Get( pt.x, pt.y ); 
  }
  
  void Set(  point_t pt, Color cl ) 
  { 
    Set( pt.x, pt.y, cl); 
  } 

private:
  Color m_field[FieldSize][FieldSize];
};


#endif // GameField_h__
