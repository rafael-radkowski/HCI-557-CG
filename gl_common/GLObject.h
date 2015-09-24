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


using namespace std;


/*!
 Global function to set the trackball data
 */
void SetTrackballLocation(glm::mat4 trackball);

/*!
 Global function to set the view matrix as look at
 */
void SetViewAsLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);



class GLMaterial
{
public:
    
    glm::vec3   _specular_material;
    glm::vec3   _diffuse_material;
    glm::vec3   _ambient_material;
    float       _shininess;
    
    
    int         _ambientColorPos;
    int         _diffuseColorPos;
    int         _specularColorPos;
    int         _shininessIdx;
    
    GLMaterial()
    {
        _specular_material = glm::vec3(1.0,0.0,0.0);
        _diffuse_material = glm::vec3(1.0,0.0,0.0);
        _ambient_material = glm::vec3(1.0,0.0,0.0);
    }
    
};


class GLLightSource
{
public:
    
    float       _specular_intensity;
    float       _diffuse_intensity;
    float       _ambient_intensity;

    
    // the glsl shader program positions
    int         _specularIdx;
    int         _diffuseIdx;
    int         _ambientIdx;
    
    glm::vec3   _lightPos;
    
    GLLightSource():
        _specular_intensity(1.0), _diffuse_intensity(1.0), _ambient_intensity(1.0)
    {
        _lightPos = glm::vec3(0.0,0.0,0.0);
        _specularIdx = _diffuseIdx =_ambientIdx  = -1;
    }
    
};




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