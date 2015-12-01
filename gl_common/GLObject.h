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

// local
#include "GLAppearance.h"

using namespace std;




/*!
 Global function to set the trackball data
 */
void SetTrackballLocation(glm::mat4 trackball);


/*!
 Global function to set the trackball data
 */
void SetTrackballLocation(glm::mat4 orientation, glm::vec3 translation);


/*!
 Global function to set the view matrix as look at
 */
void SetViewAsLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);


/*!
 Set the view frustum. 
 */
void SetPerspectiveViewFrustum(float view_angle_y, float ratio, float near, float far);


/*!
 Set the camera view matrix.
 */
void SetViewAsMatrix(glm::mat4 viewmatrix);

/*!
 Abstract base class for objects which share a common view.
 This object "common" type acts as a virtual camera and 
 stores the view and projection matrix for each object as well as 
 global functions to access them.
 */
class GLObject
{
    // Ray intersection test has access to protected functions
    friend class RayIntersectionTest;
public:
    
    // constructor
    GLObject();
    
    virtual ~GLObject();
    
    /*!
     Draw the objects
     */
    virtual void draw(void) = 0;
    
    
    /*!
     Add the model view matrix, especially the three shader program objects to
     the shader program "program"
     */
    bool addModelViewMatrixToProgram(GLuint program);
    
    
    
    /*!
     Set the appearance of this object
     */
    void setApperance(GLAppearance& apperance);
    
    
    /*!
     Set a model matrix to move the object around
     @param matrix - the model matrix for this object.
     */
    void setMatrix(glm::mat4& matrix);
    
    
    /*!
    Returns the shader program of this object
    @return, the shader program id > 0. If an appearance is not set, 
            the value is also 0.
    */
    int getProgram(void);
    
    
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
    glm::mat4 invViewMatrix(void);
    
    // returns the rotated view matrix, mulitplied with the trackball. 
    glm::mat4 rotatedViewMatrix(void);
    glm::mat4 invRotatedViewMatrix(void);
    
    // The location of teh view matrix and the projection matrix inside the shader code.
    int                     _viewMatrixLocation;
    int                     _projectionMatrixLocation;
    int                     _inverseViewMatrixLocation;
    
    // Stores the model matrix and the model matrix location
    int                     _modelMatrixLocation;
    glm::mat4               _modelMatrix; // Store the model matrix
    
    
    // The material for this object
    GLMaterial              _material;
    
    
    // An apperance object
    GLAppearance            _apperance;
    
};










