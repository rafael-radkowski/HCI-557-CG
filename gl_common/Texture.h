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


/*!
 This texture base class for textures. 
 It is just a name change to make the relation more obvious. 
 */
typedef GLVariable GLTextureBase;


/*!
 This texture class adds a texture to a primitive. 
 It is part of the appearance object and need to be used with a shader program that supports textures.
 */
class GLTexture : public GLTextureBase
{
private:
    
    // Allow the class GLApperance access to protected variables. 
    friend class GLAppearance;
    
    
    // These are the variable names which are used in our glsl shader programs.
    // Make sure that you use the correct names in your programs.
#ifdef WIN32
    static string      _glsl_names[];
#else
	const string      _glsl_names[2] = { "tex", "texture_blend"};
#endif

public:
    GLTexture();
    ~GLTexture();
    
    /*!
     Loads a texture from a file and creates the necessary texture objects
     @param path_and_file to the texture object
     @return int - the texture id when the texture was sucessfully loaded.
     */
    int loadAndCreateTexture(string path_and_file);
    
    /*!
     This sets the texture blend model
     @param mode - the values 0,1, or 2
     @return true, when a new mode was set, false when current and new mode are equal
     */
    bool setTextureBlendMode(int mode);
    
    
protected:
    
    /*!
     Adds the variables of this object to the shader program
     */
    virtual bool addVariablesToProgram(GLuint program, int variable_index);
    
    
    /*!
     The function indicates that the variables of this object require an update
     */
    virtual bool dirty(GLuint program);

    
    
    // The texture for this program.
    GLuint      _texture;
    
    // The blending mode for this texture
    int         _texture_blend_mode;

    // location of the texture in the glsl program 
    int         _textureIdx;
    int         _textureBlendModelIdx;
    
    
   
    
};





/*!
 This texture class adds two textures to a glsl shader program.
 It is part of the appearance object and need to be used with a shader program that supports textures.
 */
class GLMultiTexture : public GLTextureBase
{
private:
    // Allow the class GLApperance access to protected variables.
    friend class GLAppearance;
    
    
    // These are the variable names which are used in our glsl shader programs.
    // Make sure that you use the correct names in your programs.
#ifdef WIN32
    static string      _glsl_names[];
#else
    const string      _glsl_names[3] = { "texture_background", "texture_foreground", "texture_blend"};
#endif
    
    
    
public:
    GLMultiTexture();
    ~GLMultiTexture();
    
    /*!
     Load two bitmap images as textures from files.
     @param path_and_file_texture_1 - path and file of the first image.
     @param path_and_file_texture_1 - path and file of the second image.
     @return int - the texture id when the texture was sucessfully loaded.
     */
    int loadAndCreateTextures(string path_and_file_texture_1, string path_and_file_texture_2);
    
    /*!
     This sets the texture blend model
     @param mode - the values 0,1, or 2
     @return true, when a new mode was set, false when current and new mode are equal
     */
    bool setTextureBlendMode(int mode);
    
    
    
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
    GLuint      _texture_1;
    GLuint      _texture_2;
    
    // The blending mode for this texture
    int         _texture_blend_mode;
    
    // location of the texture in the glsl program
    int         _textureIdx1;
    int         _textureIdx2;
    int         _textureBlendModelIdx;
    
    

};


/*!
 Mipmap texture object. 
 Extends the class GLTexture.
 It loads the texture file in a slightly different way.
*/
class GLMipMapTexture : public GLTexture
{
private:
    
    // Allow the class GLApperance access to protected variables.
    friend class GLAppearance;
    
    
public:
    GLMipMapTexture();
    ~GLMipMapTexture();
    
    /*!
     Loads a texture from a file and creates the necessary texture objects
     @param path_and_file to the texture object
     @return int - the texture id when the texture was sucessfully loaded.
     */
    int loadAndCreateTexture(string path_and_file);


};




/*!
 Load a bitmap image from a file
 @param path_and_file - the path and filename
 @param data - a pointer to return the data. Note, the data object should be deleted once the data is not required anymore.
 @param channels - reference to an integer to keep the channels.
 @param width, height, - reference to integers to return the dimensions in pixels.
 */
unsigned char*  loadBitmapFile(string path_and_file, unsigned int& channels, unsigned int& width, unsigned int& height );





/*!
 Verifies wheterh a file [name] exits
 @param name - the path and the name of the file.
 @param new_name - if this variable is not empty, the file is in a different folder. 
 */
bool SearchTexture (const std::string& name, string& new_name);