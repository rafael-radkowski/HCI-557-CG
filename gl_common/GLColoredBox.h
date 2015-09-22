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

class GLColoredBox : public GLObject
{
public:
    GLColoredBox(float size_x = 1.0, float size_y = 1.0, float size_z = 1.0);
    ~GLColoredBox();
    
    
    /*!
     Draw the objects
     */
    void draw(void);
    
    
    /*!
     Set the model matrix for this object
     @param modelmatrix: 4x4 model matrix
     */
    void setModelMatrix(glm::mat4& modelmatrix);
    
    /*!
     Retursn the model matrix.
     */
    inline glm::mat4& getModelMatrix(void){return _modelMatrix;};
    
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
    int                     _modelMatrixLocation;
    
    glm::mat4               _modelMatrix; // Store the model matrix
    
    
    unsigned int            _vaoID[1]; // Our Vertex Array Object
    
    unsigned int            _vboID[1]; // Our Vertex Buffer Object
    
    // the dimensions of the box
    float                   _size_x;
    float                   _size_y;
    float                   _size_z;
};