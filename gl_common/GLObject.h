//
//  GLObject.hpp
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


/*!
 Global function to set the trackball data
 */
void SetTrackballLocation(glm::mat4 trackball);

/*!
 Global function to set the view matrix as look at
 */
void SetViewAsLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);





/*!
 Abstract base class for objects which share a common view.
 */
class GLObject
{
public:
    
    // constructor
    GLObject();
    
    virtual ~GLObject();
    
    /*!
     Draw the objects
     */
    virtual void draw(void) = 0;
    
    
    
protected:
    
    
    /*!
     Create the vertex buffer object for this element
     */
    virtual void initVBO(void) = 0;
    
    /*
     Inits the shader program for this object
     */
    virtual void initShader(void) = 0;


    // Model view projection paramaters
    glm::mat4 projectionMatrix(void);
    glm::mat4 viewMatrix(void);
    
    // returns the rotated view matrix, mulitplied with the trackball. 
    glm::mat4 rotatedViewMatrix(void);
    
};