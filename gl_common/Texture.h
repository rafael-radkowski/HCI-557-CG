//
//  Texture.hpp
//  HCI557_Simple_Texture
//
//  Created by Rafael Radkowski on 10/10/15.
//
//
#pragma once

// stl include
#include <stdio.h>
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



using namespace std;

class GLTexture : public GLVariable
{
private:
    
    // Allow the class GLApperance access to protected variables. 
    friend class GLAppearance;
    
    
    // These are the variable names which are used in our glsl shader programs.
    // Make sure that you use the correct names in your programs.
    string      _glsl_names[1] = { "tex"};
    
public:
    GLTexture();
    ~GLTexture();
    
    /*!
     Loads a texture from a file and creates the necessary texture objects
     @param path_and_file to the texture object
     @return int - the texture id when the texture was sucessfully loaded.
     */
    int loadAndCreateTexture(string path_and_file);
    
    
protected:
    
    /*!
     Adds the variables of this object to the shader program
     */
    virtual bool addVariablesToProgram(GLuint program, int variable_index);
    
    
    /*!
     The function indicates that the variables of this object require an update
     */
    virtual bool dirty(GLuint program);
    
    
private:
    
    // The texture for this program.
    GLuint      _texture;

    // location of the texture in the glsl program 
    int         _textureIdx;

};