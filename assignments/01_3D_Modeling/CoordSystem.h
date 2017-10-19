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


#include "HCI557Common.h"


using namespace std;

class CoordSystemRenderer
{
public:
    CoordSystemRenderer(float length = 1.0);
    ~CoordSystemRenderer();
    
    
    /*!
     Draw the objects
     */
    void draw(void);
    

    
private:

    
    /*!
     Create the vertex buffer object for this element
     */
    void initVBO(void);
    
    /*
     Inits the shader program for this object
     */
    void initShader(void);
    

    
    
    // the program
    GLuint                  _program;
    int                     _viewMatrixLocation;
    int                     _projectionMatrixLocation;
    
    glm::mat4               _modelMatrix; // Store the model matrix
    
    
    unsigned int            _vaoID[1]; // Our Vertex Array Object
    
    unsigned int            _vboID[1]; // Our Vertex Buffer Object
    
    // length of the line
    float                   _length;
};