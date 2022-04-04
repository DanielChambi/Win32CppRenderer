#ifndef GEOMETRY
#define GEOMETRY

#include <iostream>
#include <stdlib.h>

template<typename T>
class Vec2
{
public:
    T x, y;
    
    Vec2() : x(0), y(0) {};
    Vec2(T xx) : x(xx), y(xx) {};
    Vec2(T xx, T yy) : x(xx), y(yy) {}

    Vec2 operator + (Vec2 &v)
    {
        return Vec2(x + v.x, y + v.y);
    }
    Vec2 operator - (Vec2 &v)
    {
        return Vec2(x - v.x, y - v.y);
    }
    Vec2 operator / (T &r)
    {
        return Vec2(x / r, y / r);
    }
    Vec2 operator * (T &r)
    {
        return Vec2(x * r, y * r);
    }
    Vec2& operator += (Vec2 &v)
    {
        x += v.x; y += v.y;
        return *this;
    }
    Vec2& operator -= (Vec2 &v)
    {
        x -= v.x; y -= v.y;
        return *this;
    }
    Vec2& operator /= (T &r)
    {
        x /= r; y /= r;
        return *this;
    }
    Vec2& operator *= (T &r)
    {
        x *= r; y *= r;
        return *this;
    }

    /*
    Vec2(T xx);
    Vec2(T xx, T yy);

    Vec2 operator + (Vec2 &v);
    Vec2 operator - (Vec2 &v);
    Vec2 operator / (T &r);
    Vec2 operator * (T &r);
    Vec2& operator += (Vec2 &v);
    Vec2& operator -= (Vec2 &v);
    Vec2& operator /= (T &r);
    Vec2& operator *= (T &r);
    */
    //Vector result of multiplying/dividing vectors?

    friend std::ostream& operator << (std::ostream &s, Vec2<T> &v); 
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

template<typename T>
class Vec3
{
public:
    T x, y, z;
    /*
    Vec3();
    Vec3(T xx);
    Vec3(T xx, T yy, T zz);

    Vec3 operator + (Vec3 &v);
    Vec3 operator - (Vec3 &v);
    Vec3 operator / (T &r);
    Vec3 operator * (T &r);
    Vec3& operator += (Vec3 &v);
    Vec3& operator -= (Vec3 &v);
    Vec3& operator /= (T &r);
    Vec3& operator *= (T &r);
    T DotProduct(Vec3 &v);
    Vec3 CrossProduct(Vec3<T> &v);

    T SqrdLength();
    T Length();

    T& operator [] (uint8_t i);

    Vec3& Normalize();

    //Vector result of multiplying/dividing vectors?
    */

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

    friend std::ostream& operator << (std::ostream &s, Vec3<T> &v); 
};

typedef Vec3<float> Vec3f;

template<typename T>
class Matrix44
{
public:
    T x[4][4] = {{1, 0, 0, 0}, 
                 {0, 1, 0, 0}, 
                 {0, 0, 1, 0}, 
                 {0, 0, 0, 1}};

    /*
    Matrix44();

    Matrix44(T a, T b, T c, T d,
             T e, T f, T g, T h,
             T i, T j, T k, T l,
             T m, T n, T o, T p);

    T* operator [] (uint8_t i);

    Matrix44 operator * (Matrix44 &v);

    static void Multiply(Matrix44<T> &a, Matrix44<T> &b, Matrix44<T> &result);

    Matrix44 Transposed();

    //Our implementation doesn't differentiate vectors from points and normals
    //As multiplication between points and vectors have to be treated
    //Differently we define different functions
    template <typename S>
    void MultPointMatrix(Vec3<S> &src, Vec3<S> &dst);

    template <typename S>
    void MultVecMatrix(Vec3<S> &src, Vec3<S> &dst);

    Matrix44 Inverse();

    //function to invert self?
    */

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
        return result;
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

        a = src.x * x[0][0] + src.y * x[1][0] + src.z * x[2][0];
        b = src.x * x[0][1] + src.y * x[1][1] + src.z * x[2][1];
        c = src.x * x[0][2] + src.y * x[1][2] + src.z * x[2][2];

        dst.x = a;
        dst.y = b;
        dst.z = c;
    }

    Matrix44 Inverse()
    {
        Matrix44 Temp(* this);
        Matrix44 Result;
        //Using Gauss-Jordan method

        //Scale first row and substract from other rows
        //This makes the first columns all zeros(except x[0][0])

        //Loop to scale diagonal to value 1 and neutralize(make value 0) columns downwards
        for(int n = 0; n < 3; n++)
        {
            //Scale row so Temp[n][n] is 1
            if(Temp[n][n] != 1)
            {
                float Factor = 1/Temp[n][n];
                for(int i = 0; i < 4; i++)
                {
                    //Do same operation on both matrixes
                    Temp[n][i] *= Factor;
                    Result[n][i] *= Factor;
                }
            }

            //Scale row and substract from other rows to neutralize column
            for(int i = n + 1; i < 4; i++)
            {
                if(Temp[i][n] != 0)
                {
                    float Factor = Temp[i][n];
                    for(int j = 0; j < 4; j++)
                    {
                        Temp[i][j] -= Temp[n][j]*Factor;
                        Result[i][j] -= Result[n][j]*Factor;
                    }
                }
            }
        }

        //Scale fourth row to make Temp[3][3] equal to 1
        if(Temp[3][3] != 1)
        {
            float Factor = 1/Temp[3][3];
            Temp[3][3] = 1;
            for(int i = 0; i < 4; i++)
            {
                Result[3][i] *= Factor;
            }
        }

        //Neutralize columns upwards
        for(int n = 3; n >= 1; n--)
        {
            for(int i = n - 1; i >= 0; i--)
            {
                if(Temp[i][n] != 0)
                {
                    float Factor = Temp[i][n];
                    Temp[i][n] = 0;
                    for(int j = 0; j < 4; j++)
                    {
                        Result[i][j] -= Result[n][j]*Factor;
                    }
                }
            }
        }

        return Result;
    }

    friend std::ostream& operator << (std::ostream &s, Matrix44 m);
};

typedef Matrix44<float> Matrix44f;

#endif