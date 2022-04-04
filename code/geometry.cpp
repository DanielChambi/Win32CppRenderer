#include "geometry.h"

#include <iostream>
#include <stdlib.h>
/*

   Vec2() : x(0), y(0) {};
   Vec2(T xx) : x(xx), y(xx) {};
   Vec2(T xx, T yy) : x(xx), y(yy) {};

  Vec2  operator + (Vec2 &v)
{
    return Vec2(x + v.x, y + v.y);
}
  Vec2  operator - (Vec2 &v)
{
    return Vec2(x - v.x, y - v.y);
}
  Vec2  operator / (T &r)
{
    return Vec2(x / r, y / r);
}
  Vec2  operator * (T &r)
{
    return Vec2(x * r, y * r);
}
  Vec2&  operator += (Vec2 &v)
{
    x += v.x; y += v.y;
    return *this;
}
  Vec2&  operator -= (Vec2 &v)
{
    x -= v.x; y -= v.y;
    return *this;
}
  Vec2&  operator /= (T &r)
{
    x /= r; y /= r;
    return *this;
}
  Vec2&  operator *= (T &r)
{
    x *= r; y *= r;
    return *this;
}
*/

/*
  std::ostream&  operator << (std::ostream &s, Vec2 &v)
{
    return << s << '[' << v.x << ' ' << v.y << ']';
} 
*/
/*
  Vec3() : x(0), y(0), z(0) {};
  Vec3(T xx) : x(xx), y(xx), z(xx) {};
  Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {};

  Vec3 operator + (Vec3 &v)
{
    return Vec3(x + v.x, y + v.y, z + v.z);
}
  Vec3 operator - (Vec3 &v)
{
    return Vec3(x - v.x, y - v.y, z - v.z);
}
  Vec3 operator / (T &r)
{
    return Vec3(x / r, y / r, z / r);
}
  Vec3 operator * (T &r)
{
    return Vec3(x * r, y * r, z * r);
}
  Vec3& operator += (Vec3 &v)
{
    x += v.x; y += v.y; z += v.z;
    return *this;
}
  Vec3& operator -= (Vec3 &v)
{
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
}
  Vec3& operator /= (T &r)
{
    x /= r; y /= r; z /= r;
    return *this;
}
  Vec3& operator *= (T &r)
{
    x *= r; y *= r; z *= r;
    return *this;
}
  T DotProduct(Vec3 &v)
{
    return x * v.x + y * v.y + z * v.z;
}
  Vec3 CrossProduct(Vec3 &v)
{
    return Vec3(y * v.z - v.y * z, z * v.x - v.z * x, x * v.y - v.x * y);
}

  T SqrdLength()
{
    return x*x + y*y + z*z;
}
  T Length()
{
    return sqrt( SqrdLength() );
}

  T& operator [] (uint8_t i)
{
    return (&x)[i];
}

  Vec3& Normalize()
{
    T SLenght = SqrdLength();
    if(SLenght > 0)
    {
        T length = sqrt(SLenght);
        x /= length; y /= length; z /length;
    }
    return *this;
}
*/

/*
  Vec3::std::ostream& << (std::ostream &s, Vec3 &v)
{
    return << s << '[' << v.x << ' ' << v.y << ' ' << v.z << '[]';
}
*/

/*
  Matrix44() {};

  Matrix44(T a, T b, T c, T d,
                                        T e, T f, T g, T h,
                                        T i, T j, T k, T l,
                                        T m, T n, T o, T p)
{
    x[0][0] = a; x[0][1] = b; x[0][2] = c; x[0][3] = d;
    x[1][0] = e; x[1][1] = f; x[1][2] = g; x[1][3] = h;
    x[2][0] = i; x[2][1] = j; x[2][2] = k; x[2][3] = l;
    x[3][0] = m; x[3][1] = n; x[3][2] = o; x[3][3] = p;
}

  T* operator [] (uint8_t i)
{
    return x[i];
}

  Matrix44 operator * (Matrix44 &v)
{
    Matrix44 result;
    Multiply(*this, v, result);
    return result;

}

  void Multiply(Matrix44 &a, Matrix44 &b, Matrix44 &result)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            result[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] +
                           a[i][2] * b[2][j] + a[i][3] * b[3][j]; 
        }
    }
}

  Matrix44 Transposed()
{
    Matrix44 result;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            result[i][j] = x[j][i];
        }
    }
}

    //Our implementation doesn't differentiate vectors from points and normals
    //As multiplication between points and vectors has to be treated
    //Differently we define different functions
 
template<typename S> void MultPointMatrix(Vec3<S> &src, Vec3<S> &dst)
{
    T a, b, c, w;

    //In point multiplication the fourth column of the matrix is the translation vector
    a = src.x * x[0][0] + src.y * x[1][0] + src.z * x[2][0] + x[3][0];
    b = src.x * x[0][1] + src.y * x[1][1] + src.z * x[2][1] + x[3][1];
    c = src.x * x[0][2] + src.y * x[1][2] + src.z * x[2][2] + x[3][2];

    //w is a factor in the transformation matrix that has to be always 1
    w = src.x * x[0][3] + src.y * x[1][3] + src.z * x[2][3] + x[3][3];

    //In case w is different from 1 we scale the result by the factor 1/w
    dst.x = a / w;
    dst.y = b / w;
    dst.z = c / w;
}

 
template<typename S> void MultVecMatrix(Vec3<S> &src, Vec3<S> &dst)
{
    T a, b, c;

    a = src.x * x[0][0] + src.y * x[1][0] + src.z * x[2][0] + x[3][0];
    b = src.x * x[0][1] + src.y * x[1][1] + src.z * x[2][1] + x[3][1];
    c = src.x * x[0][2] + src.y * x[1][2] + src.z * x[2][2] + x[3][2];

    dst.x = a;
    dst.y = b;
    dst.z = c;
}

  Matrix44 Inverse();
*/

/*
  Matrix44::std::ostream& operator << (std::ostream &s, Matrix44 m)
{
    int width = 10;

    for(int i = 0; i < 4; i++)
    {
        s << '[';
        for(int j = 0; j < 4; j++)
        {
            s << std::setw(10) << m[i][j] << ' ';
        }
        s << ']' << '\n';
    }

    return s << '\n';
}
*/

//template Matrix44<float>::Matrix44<float>();

void testC::test5(){
    int x = 0;
}