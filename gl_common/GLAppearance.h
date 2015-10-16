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

// locals
#include "GLAppearanceBase.h"
#include "Texture.h"


using namespace std;




/*!
 A Material class, which allows us to define a material
 */
class GLMaterial : public GLVariable
{
protected:
    
    // These are the variable names which are used in our glsl shader programs.
    // Make sure that you use the correct names in your programs.
#ifdef WIN32
    static string      _glsl_names[]; 
    static string      _glsl_struct;
#else
	const string      _glsl_names[6] = { "ambient", "diffuse",  "specular",  "shininess", "emissive", "transparency"};
    const string      _glsl_struct = "allMaterials";
#endif
    
public:
    
    // The material parameters
    glm::vec3   _specular_material;
    glm::vec3   _diffuse_material;
    glm::vec3   _ambient_material;
    glm::vec3   _emissive_material;
    
    // Shininess factor
    float       _shininess;

    // the transparency coefficient
    float       _transparency;
    
    // The shader program indices of the material parameters.
    int         _ambientColorPos;
    int         _diffuseColorPos;
    int         _specularColorPos;
    int         _shininessIdx;
    int         _transparencyIdx;
    int         _emissiveIdx;
    
    GLMaterial()
    {
        _specular_material = glm::vec3(1.0,0.0,0.0);
        _diffuse_material = glm::vec3(1.0,0.0,0.0);
        _ambient_material = glm::vec3(1.0,0.0,0.0);
        _emissive_material = glm::vec3(0.0,0.0,0.0);
        _shininess = 1.0;
        _transparency = 1.0;
    }
    
    /*!
     Add all the variables of this material object to the shader program "program".
     It expects that the program already exits and that the names in _glsl_names are used
     @param program - the glsl shader program integer id
     */
    virtual bool addVariablesToProgram(GLuint program, int variable_index);
    
    
    /*!
     Updates all the variables with new values
     */
    virtual bool dirty(GLuint program);
};



/*
 A light source class, which allows us to represent a light source.
 */
class GLLightSource  : public GLVariable
{
protected:
    
    // These are the variable names which are used in our glsl shader programs.
    // Make sure that you use the correct names in your programs.
#ifdef WIN32
    static string      _glsl_names[];
    static string      _glsl_object;
#else
	const string       _glsl_names[5] = { "specular_intensity", "diffuse_intensity",  "ambient_intensity",  "attenuationCoefficient", "light_position"};
    const string       _glsl_object = "allLights";
#endif
    
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
    
    
    
    /*!
     Add all the variables of this material object to the shader program "program".
     It expects that the program already exits and that the names in _glsl_names are used
     @param program - the glsl shader program integer id
     */
    virtual bool addVariablesToProgram(GLuint program, int variable_index);
    
    
    
    /*!
     Updates all the variables with new values
     */
    virtual bool dirty(GLuint program);
    
    
};



/*!
 A spot light source which adds a cone angle and 
 cone direction as parameters to the light source
 */
class GLSpotLightSource : public GLLightSource
{
protected:
    
    // These are the variable names which are used in our glsl shader programs.
    // Make sure that you use the correct names in your programs.
#ifdef WIN32
    static string      _glsl_names[];
#else
	const string       _glsl_names[2] = { "cone_angle", "cone_direction"};
#endif

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


    
    /*!
     Add all the variables of this material object to the shader program "program".
     It expects that the program already exits and that the names in _glsl_names are used
     @param program - the glsl shader program integer id
     */
    virtual bool addVariablesToProgram(GLuint program, int variable_index);
    
    
    
    /*!
     Updates all the variables with new values
     */
    virtual bool dirty(GLuint program);
    
};




/*!
 A point light source
 */
typedef GLLightSource GLPointLightSource;

/*!
 A direct light source
 */
typedef GLLightSource GLDirectLightSource;


/*!
 This object defines the apperance of a 3D geometry model.
 It combines the shader code, with the material, and the light sources
 */
class GLAppearance
{
public:
    
    // Create an apperance object using a vertex and fragment shader file
    GLAppearance(string vertex_shader_file, string fragment_shader_file);
    GLAppearance();
    ~GLAppearance();
    
    /*!
     Returns the program index
     */
    inline GLuint getProgram(void){return _program;}
    
    
    /*!
     Add a light source that should illuminate this object.
     You can add multiple light sources
     @param light_source - a light source reference
     */
    void addLightSource(GLLightSource& light_source);
    
    /*!
     Set the material for this object
     @param material - a reference to a material object
     */
    void setMaterial(GLMaterial& material);
    
    /*!
     Helper function to check whether this objects exits. 
     */
    inline bool exists(void){return _exists;}
    
    /*!
     Update all the materials if values changes
     */
    void updateMaterial(void);
    
    /*!
     Updates all the light sources
     */
    void updateLightSources(void);
    
    
    /*!
     Updates all texture variables
     */
    void updateTextures(void);

    
    /*!
     Finalize the program and all variables.
     */
    void finalize(void);
    
    
    /*!
     Add a texture to the appearance.
     */
    void setTexture(GLTexture* texture);
    void setTexture(GLMultiTexture* texture);
    
protected:
    
    /*!
     Add all the variables of this material object to the shader program "program".
     It expects that the program already exits and that the names in _glsl_names are used
     */
    bool addVariablesToProgram(void);
    
    
private:

    
    
    // The shader program
    GLuint                      _program;
    
    
    // The material for this object
    GLMaterial*                 _material;
    
    
    // vector to store all the light sources that illuminate this object
    vector<GLLightSource*>      _light_sources;
    
    
    // maintains the texture objectx
    vector<GLTextureBase*>          _textures;
    
    
    // counts the number of light sources
    int                         _num_light_sources;
    
    
    // helper to verify whether this object is ok.
    bool                        _exists;
    
    
    // helper to mark the appearance as complete
    bool                        _finalized;

};






