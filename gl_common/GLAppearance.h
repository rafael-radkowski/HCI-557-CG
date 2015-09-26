//
//  GLAppearance.hpp
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



/*!
 A Material class, which allows us to define a material
 */
class GLMaterial
{
public:
    
    // The material parameters
    glm::vec3   _specular_material;
    glm::vec3   _diffuse_material;
    glm::vec3   _ambient_material;
    
    // Shininess factor
    float       _shininess;

    
    // The shader program indices of the material parameters.
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

/*
 A light source class, which allows us to represent a light source.
 */
class GLLightSource
{
public:
    
    // The intensity of the light source
    float       _specular_intensity;
    float       _diffuse_intensity;
    float       _ambient_intensity;
    
    // The position of the light source
    glm::vec4   _lightPos;
    
    
    // Attentuation
    float       _attenuation_coeff; // the strength of the attenuation
    
    
    // the glsl shader program positions
    int         _specularIdx;
    int         _diffuseIdx;
    int         _ambientIdx;
    int         _lightPosIdx;
    int         _attenuation_coeffIdx;
    
    GLLightSource():
    _specular_intensity(1.0), _diffuse_intensity(1.0), _ambient_intensity(1.0)
    {
        _lightPos = glm::vec4(0.0,0.0,0.0,1.0);
        _specularIdx = _diffuseIdx =_ambientIdx = _attenuation_coeffIdx  = -1;
    }
    
    
    /*!
     Set the position of the light source in x, y, z.
     @param x,y,z - the position of the light source
     */
    inline void setPosition(float x, float y, float z){
        _lightPos[0] = x; _lightPos[1] = y; _lightPos[2] = z; _lightPos[3] = 1.0;
    }
    
    /*!
     Set the intensity of the specular light
     @param intensity - an intensity value > 0.0;
     */
    inline void setSpecular(float intensity){
        if (intensity < 0.0) return;
        _specular_intensity = intensity;
    }
    
    /*!
     Set the intensity of the diffuse light
     @param intensity - an intensity value > 0.0;
     */
    inline void setDiffuse(float intensity){
        if (intensity < 0.0) return;
        _diffuse_intensity = intensity;
    }
    
    
    /*!
     Set the intensity of the ambient light
     @param intensity - an intensity value > 0.0;
     */
    inline void setAmbient(float intensity){
        if (intensity < 0.0) return;
        _ambient_intensity = intensity;
    }
    
    /*!
     Set the intensity for the specular and the diffuse component
     @param intensity - an intensity value > 0.0;
     */
    inline void setIntensity(float intensity){
        setSpecular(intensity);
        setDiffuse(intensity);
    }
    
    /*!
     Returns the diffuse, specular, and ambient intensity.
     */
    inline float diffuse(void){return _diffuse_intensity;}
    inline float specular(void){return _specular_intensity;}
    inline float ambient(void){return _ambient_intensity;}
    
    
};


/*!
 A spot light source which adds a cone angle and 
 cone direction as parameters to the light source
 */
class GLSpotLightSource : public GLLightSource
{
public:
    
    // The cone angle in degree because we calculate the angle in degree in our glsl program
    float       _cone_angle;
    
    // The direction of the light
    glm::vec3   _cone_direction;
    
    // the glsl shader indices
    int         _cone_angleIdx;
    int         _cone_directionIdx;
    
    GLSpotLightSource()
    {
        // some default values.
        _cone_angle = 10.0;
        _cone_direction = glm::vec3(1.0,0.0,0.0);
    }


};







