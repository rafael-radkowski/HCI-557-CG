#pragma once

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



using namespace std;



/**
 A vertex datatype, a helper to define points in 3D space
 */
template <class T>
class _Vertex{
    
protected:
    
    // The data of this vertex
    T _x;
    T _y;
    T _z;
    T _w;
    
    
    // Texture coordinates
    T _u;
    T _v;

    
public:
    _Vertex(T px, T py, T pz):
    _x(px), _y(py), _z(pz), _w(1.0), _u(0.0), _v(0.0){};
    
    _Vertex():
    _x(0.0), _y(0.0), _z(0.0), _w(1.0), _u(0.0), _v(0.0){};
    
    
    inline _Vertex operator-(_Vertex& r)
    {
        _Vertex res;
        res._x = _x-r._x;
        res._y = _y-r._y;
        res._z = _z-r._z;
        return res;
    }
    
    inline _Vertex operator+(_Vertex& r)
    {
        _Vertex res;
        res._x = _x+r._x;
        res._y = _y+r._y;
        res._z = _z+r._z;
        return res;
    }
    
    inline _Vertex& operator*(float r)
    {
        _x = _x*r;
        _y = _y*r;
        _z = _z*r;
        return *this;
    }
    
    inline _Vertex& operator/(float r)
    {
        _x = _x/r;
        _y = _y/r;
        _z = _z/r;
        return *this;
    }
    
    inline void operator=(_Vertex& r)
    {
        _Vertex res;
        _x = r._x;
        _y = r._y;
        _z = r._z;
    }
    
    inline void normalize(void)
    {
        T l = length();
        _x = _x / l;
        _y = _y / l;
        _z = _z / l;
        
    }
    
    inline float length(void)
    {
        return sqrt( T((_x*_x) + (_y*_y) + (_z*_z)) );
    }
    
    
    /*!
     Calculates the cross product and returns the result as new vertex
     */
    inline static _Vertex cross_product(_Vertex& v1, _Vertex& v2)
    {
        _Vertex r;
        
        r.x() = v1.y() * v2.z() - v1.z() * v2.y();
        r.y() = v1.z() * v2.x() - v1.x() * v2.z();
        r.z() = v1.x() * v2.y() - v1.y() * v2.x();
        
        return r;
    }
    
    inline void cross_product(_Vertex& v)
    {
        T tx, ty, tz;
        tx = y() * v.z() - z() * v.y();
        ty = z() * v.x() - x() * v.z();
        tz = x() * v.y() - y() * v.x();
        
        _x = tx; _y = ty; _z = tz;
    }
    
    /*!
     To access the values 
     */
    inline T& x(){return _x;}
    inline T& y(){return _y;}
    inline T& z(){return _z;}
    inline T& w(){return _w;}
    
    inline T& u(){return _u;}
    inline T& v(){return _v;}
    
};


/*
 Defines a standard datatype vertex using float for _Vertex
 */
typedef _Vertex<float> Vertex;








