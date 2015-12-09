//
//  GLColoredBox.h
//  OpenGL_Transformations
//
//  Created by Rafael Radkowski on 9/12/15.
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


#include "GLObject.h"



using namespace std;

class GLSurface : public GLObject
{
    // Ray intersection test has access to protected functions
    friend class RayIntersectionTest;
    
public:
    GLSurface( vector<glm::vec3> &vertices, vector<glm::vec3> &normals);
    ~GLSurface();
    
    
    /*!
     Draw the objects
     */
    void draw(void);
    
    
    /*!
     Init the geometry object
     */
    void init(void);
    
    
    /*!
    Returns the number of vertices
    */
    int size(void);
    
    

    
private:
    
    
    
    /*!
     Create the vertex buffer object for this element
     */
    virtual void initVBO(void);
    
    /*
     Inits the shader program for this object
     */
    virtual void initShader(void);
    
    
    // the program
    GLuint                  _program;
    

protected:
    // The data
    vector<glm::vec3>       _vertices;
    vector<glm::vec4>       _vertex_colors;
    vector<glm::vec3>       _normals;
    
private:
    
    int                     _num_vertices;
    
    unsigned int            _vaoID[1]; // Our Vertex Array Object
    
    unsigned int            _vboID[2]; // Our Vertex Buffer Object

};