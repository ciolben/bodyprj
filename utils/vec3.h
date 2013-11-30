#ifndef VEC3_H
#define VEC3_H

/***************************************************************************
* Vec3.h                                                                   *
*                                                                          *
* Vec3 is a trivial encapsulation of 3D floating-point coordinates.        *
* It has all of the obvious operators defined as inline functions.         *
*                                                                          *
* History:                                                                 *
*   10/21/2003  Updated for intro graphics class.                          *
*   04/01/2003  Initial coding.                                            *
*                                                                          *
***************************************************************************/

/*
 * Changes (26.11.13) :
 *  Vec3 -> vec3
 *  double -> float
 *
 */

#include <math.h>
#include <iostream>

class vec3 {
public:
    float x;
    float y;
    float z;
    vec3() { x = 0; y = 0; z = 0; }
    vec3( float a, float b, float c ) { x = a; y = b; z = c; }
    vec3( float a, float b) { x = a; y = b; z = 0.f; }
    vec3( float a ) { x = a; y = a; z = a; }

    float* data() { float* res = new float[3]; res[0] = x; res[1] = y; res[2] = z; return res; }

    float operator[] (unsigned int i) const { return (i == 0) ? x : (i == 1) ? y : z; }
    float& operator[] (unsigned int i) { return (i == 0) ? x : (i == 1) ? y : z; }

    };

inline float normSqrt( const vec3 &A )
    {
    return A.x * A.x + A.y * A.y + A.z * A.z;
    }

inline float norm( const vec3 &A )
    {
    return sqrt( normSqrt( A ) );
    }

inline vec3 operator+( const vec3 &A, const vec3 &B )
    {
    return vec3( A.x + B.x, A.y + B.y, A.z + B.z );
    }

inline vec3 operator+( const vec3 &A, const float& f )
    {
    return vec3( A.x + f, A.y + f, A.z + f );
    }

inline vec3 operator+( const float& f, const vec3 &B )
    {
    return operator+(B, f);
    }

inline vec3 operator-( const vec3 &A, const vec3 &B )
    {
    return vec3( A.x - B.x, A.y - B.y, A.z - B.z );
    }

inline vec3 operator-( const vec3 &A, const float& f )
    {
    return vec3( A.x - f, A.y - f, A.z - f );
    }

inline vec3 operator-( const float& f, const vec3 &B )
    {
    return vec3( f - B.x, f - B.y, f - B.z );
    }

inline vec3 operator-( const vec3 &A )  // Unary minus.
    {
    return vec3( -A.x, -A.y, -A.z );
    }

inline vec3 operator*( float a, const vec3 &A )
    {
    return vec3( a * A.x, a * A.y, a * A.z );
    }

inline vec3 operator*( const vec3 &A, float a )
    {
    return vec3( a * A.x, a * A.y, a * A.z );
    }

inline vec3 operator/( const vec3 &A, float c )
    {
    return vec3( A.x / c, A.y / c, A.z / c );
    }

inline float operator*( const vec3 &A, const vec3 &B )  // Dot product.
    {
    return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
    }

inline float dot( const vec3 &A, const vec3 &B )  // Dot product.
    {
    return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
    }

inline vec3 operator^( const vec3 &A, const vec3 &B ) // Cross product.
    {
    return vec3(
        A.y * B.z - A.z * B.y,
        A.z * B.x - A.x * B.z,
        A.x * B.y - A.y * B.x
        );
    }

inline vec3 &operator+=( vec3 &A, const vec3 &B )
    {
    A.x += B.x;
    A.y += B.y;
    A.z += B.z;
    return A;
    }

inline vec3 &operator-=( vec3 &A, const vec3 &B )
    {
    A.x -= B.x;
    A.y -= B.y;
    A.z -= B.z;
    return A;
    }

inline vec3 &operator*=( vec3 &A, float a )
    {
    A.x *= a;
    A.y *= a;
    A.z *= a;
    return A;
    }

inline vec3& operator/=( vec3 &A, float a )
    {
    A.x /= a;
    A.y /= a;
    A.z /= a;
    return A;
    }

inline vec3 normalize( const vec3 &A ) // Return a normalized vector (if it is non-zero).
    {
    float d = normSqrt( A );
    return d > 0.0 ? A / sqrt(d) : vec3(0,0,0);
    }

inline vec3 OrthogonalTo( const vec3 &A )  // Return a non-zero vector orthogonal to A.
    {
    if( A.x == 0 ) return vec3( 1, 0, 0 );
    return vec3( A.y, -A.x, 0 );
    }

inline std::ostream& operator<<(std::ostream& os, const vec3& v)
{
    os << '(' << v[0] << ", " << v[1] << ", " << v[2] << ')';
    return os;
}

#endif // VEC3_H
