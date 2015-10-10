//
//  GLSphereExt.hpp
//  HCI557_Spotlight
//
//  Created by Rafael Radkowski on 9/26/15.
//
//
#pragma once


// stl include
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// locals
#include "GLObject.h"
#include "GLSphere.h"
#include "Shaders.h"

#include "HCI557Datatypes.h"





class GLPlane3D : public GLObject
{
public:
    
    GLPlane3D(float center_x, float center_y, float center_z, float width, float height,  int rows = 10, int cols = 10 );
    ~GLPlane3D();
    

    /*!
     Init the geometry object
     */
    void init(void);
    
    
    
    /*!
     Draw the objects
     */
    virtual void draw(void);
    
    
protected:
    
    
    
    /*!
     Create the vertex buffer object for this element
     */
    virtual void initVBO(void);
    
    
    /*
     Inits the shader program for this object
     */
    virtual void initShader(void);
    
    
    /*!
     Creates the points and the normal vectors for this box
    */
    void make_box(float center_x, float center_y, float center_z, float width, float height,  int rows, int cols, vector<Vertex>& vp, vector<Vertex>& nv );
    
    
    // Parameters
    float           _center_x;
    float           _center_y;
    float           _center_z;
    float           _width;
    float           _height;
    int             _rows;
    int             _cols;

    // variables to generate the sphere
    vector<Vertex>          _vertex_points;
    vector<Vertex>          _normal_vectors;
    int                     _num_vertices;
    
    int                     _tex_idx;
    
    // the program
    GLuint                  _program;
    
    unsigned int            _vaoID[1]; // Our Vertex Array Object
    unsigned int            _vboID[2]; // Our Vertex Buffer Object


};











