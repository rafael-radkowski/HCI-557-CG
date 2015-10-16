//
//  Texture.cpp
//  HCI557_Simple_Texture
//
//  Created by Rafael Radkowski on 10/10/15.
//
//

#include "Texture.h"

#ifdef WIN32
string  GLTexture::_glsl_names[2] = { "tex", "texture_blend"};

string  GLMultiTexture::_glsl_names[3] = { "texture_background", "texture_foreground", "texture_blend"};
#endif

GLTexture::GLTexture()
{
    _textureIdx = -1;
    _texture_blend_mode = 0;
    _dirty = false;
}

GLTexture::~GLTexture()
{


}

/*!
 Loads a texture from a file and creates the necessary texture objects
 @param path_and_file to the texture object
 @return int - the texture id when the texture was sucessfully loaded.
 */
int GLTexture::loadAndCreateTexture(string path_and_file)
{
    // Check whether we load a bitmap file
    
    int idx = path_and_file.find_last_of(".");
    string suffix = path_and_file.substr(idx+1, 3);
    
    if (suffix.compare("bmp") != 0) {
        cerr << "[ERROR] Filetype " << suffix << " is currently not supported. This example only support bitmap files. " << endl;
        return -1;
    }


	string checked_path_and_file;
	bool ret = SearchTexture(path_and_file, checked_path_and_file);
    
	if(!ret)
	{
		cerr << "[ERROR] Cannot find the file " << path_and_file << "." << endl;
        return -1;
	}
    
    //**********************************************************************************************
    // Loads the file content
    
    int channels = 3;
    unsigned char * data;
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;
    
    // This opens a file
    FILE * file;
    file = fopen( checked_path_and_file.c_str(), "rb" );
    
    if ( file == NULL ) return 0;
    
    // This reads the header of the file and checks the length.
    if ( fread(header, 1, 54, file)!=54 )
    {
        // If not 54 bytes read, this is not a bmp.
        // Only a bmp has a header of length 54
        printf("Not a correct BMP file\n");
        return false;
    }
    
    // Read the start position of the data, the size, the width, and height.
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    channels = imageSize / (width * height);
    
    // Create memory for this texture
    data = (unsigned char *)malloc( width * height * channels );
    
    // Read the data from a file.
    fread( data, width * height * channels, 1, file );
    
    // Release the file.
    fclose( file );
    
    
    //**********************************************************************************************
    // Texture generation
    
    // Generate a texture, this function allocates the memory and
    // associates the texture with a variable.
    glGenTextures(1, &_texture );
    
    // Set a texture as active texture.
    glBindTexture( GL_TEXTURE_2D, _texture );
    
    // Change the parameters of your texture units.
    //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_BLEND );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    
    
    // Create a texture and load it to your graphics hardware. This texture is automatically associated
    // with texture 0 and the textuer variable "texture" / the active texture.
    if(channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    else if(channels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    
    //**********************************************************************************************
    // Create a midmap texture pyramid and load it to the graphics hardware.
    // Note, the MIN and MAG filter must be set to one of the available midmap filters.
    //gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
    
    // Delete your loaded data
    free( data );
    
    // Return the texture.
    return _texture;

}




/*!
 Adds the variables of this object to the shader program
 */
//virtual
bool GLTexture::addVariablesToProgram(GLuint program, int variable_index )
{

    if(program == -1)return false; // no program exits
    
    
    GLint params;
    glGetProgramiv( program, GL_LINK_STATUS, &params);
    if(params == GL_FALSE)
    {
        cerr << "[GLTexture] Program " << program << " has not been linked. Textures cannot be added." << endl;
        return false;
    }
    
    // enable the program
    glUseProgram(program);
    
    
    // get the location of a uniform variable. Note, the program must be linked at this position.
    // location of the texture in the glsl program
    _textureIdx = glGetUniformLocation(program, _glsl_names[0].c_str() );
    checkUniform(_textureIdx, _glsl_names[0]);
    
    _textureBlendModelIdx = glGetUniformLocation(program, _glsl_names[1].c_str() );
    checkUniform(_textureBlendModelIdx, _glsl_names[1]);
    
    //****************************************************************************************************
    // Link the texture to the uniform variable and texture unit 0;
    
    /*
     glActiveTexture tells OpenGL which texture unit we want to use. GL_TEXTURE0 is the first texture unit, so we will just use that.
     */
    glActiveTexture(GL_TEXTURE0);
    
    
    //We use glBindTexture bind our texture into the active texture unit.
    glBindTexture(GL_TEXTURE_2D, _texture);
    
    /*
     Then we set the tex uniform of the shaders to the index of the texture unit. We used texture unit zero, so we set the tex uniform to the integer value 0.
     */
    glUniform1i(_textureIdx, 0);
    
    
    // update the variable
    dirty(program);
    
    return true;
}


/*!
 The function indicates that the variables of this object require an update
 */
//virtual
bool GLTexture::dirty(GLuint program)
{
    // enable the program
    glUseProgram(program);
 
    // write the texture blend mode
    glUniform1i(_textureBlendModelIdx, _texture_blend_mode);
    
    
    // disable the program
    glUseProgram(0);
    
    _dirty = false;

    return true;
}


/*!
 This sets the texture blend model
 @param mode - the values 0,1, or 2
 */
bool GLTexture::setTextureBlendMode(int mode)
{
    if(mode < 0 || mode > 2)return false;
    if(_texture_blend_mode == mode)return false;
    
    // write the texture blend mode
    _texture_blend_mode = mode;
    
    _dirty = true;

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GLMultiTexture

GLMultiTexture::GLMultiTexture()
{
    
    _texture_1 = 0;
    _texture_2 = 0;
    
    _texture_blend_mode = 0;
    _textureIdx1 = -1;
    _textureIdx2 = -1;
    _textureBlendModelIdx = -1;
    
    _dirty = false;

}


GLMultiTexture::~GLMultiTexture()
{

}

/*!
 Load two bitmap images as textures from files.
 @param path_and_file_texture_1 - path and file of the first image.
 @param path_and_file_texture_1 - path and file of the second image.
 @return int - the texture id when the texture was sucessfully loaded.
 */
int GLMultiTexture::loadAndCreateTextures(string path_and_file_texture_1, string path_and_file_texture_2)
{

    unsigned int channels1;
    unsigned int width1;
    unsigned int height1;
    
    unsigned int channels2;
    unsigned int width2;
    unsigned int height2;
    
    unsigned char* data1 = loadBitmapFile(path_and_file_texture_1, channels1, width1,  height1 );
    unsigned char* data2 = loadBitmapFile(path_and_file_texture_2, channels2, width2,  height2 );
    
    if(data1 == NULL || data2 == NULL)return -1;
    
    
    
    
    
    //**********************************************************************************************
    // Texture generation - background
    
    // Activate a texture unit
    glActiveTexture(GL_TEXTURE0);
    
    // Generate a texture, this function allocates the memory and
    // associates the texture with a variable.
    glGenTextures(1, &_texture_1 );
    
    // Set a texture as active texture.
    glBindTexture( GL_TEXTURE_2D, _texture_1 );
    
    // Change the parameters of your texture units.
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    
    
    // Create a texture and load it to your graphics hardware. This texture is automatically associated
    // with texture 0 and the textuer variable "texture" / the active texture.
    if(channels1 == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_BGR, GL_UNSIGNED_BYTE, data1);
    else if(channels1 == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_BGRA, GL_UNSIGNED_BYTE, data1);
    
    //**********************************************************************************************
    // Create a midmap texture pyramid and load it to the graphics hardware.
    // Note, the MIN and MAG filter must be set to one of the available midmap filters.
    //gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
    
    // Delete your loaded data
    free( data1 );
    
    
    
    
    //**********************************************************************************************
    // Texture light
    
    // Activate a texture unit
    glActiveTexture(GL_TEXTURE1);
    
    // Generate a texture, this function allocates the memory and
    // associates the texture with a variable.
    glGenTextures(1, &_texture_2 );
    
    // Set a texture as active texture.
    glBindTexture( GL_TEXTURE_2D, _texture_2 );
    
    // Change the parameters of your texture units.
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    
    
    // Create a texture and load it to your graphics hardware. This texture is automatically associated
    // with texture 0 and the textuer variable "texture" / the active texture.
    if(channels2 == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_BGR, GL_UNSIGNED_BYTE, data2);
    else if(channels2 == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_BGRA, GL_UNSIGNED_BYTE, data2);
    
    //**********************************************************************************************
    // Create a midmap texture pyramid and load it to the graphics hardware.
    // Note, the MIN and MAG filter must be set to one of the available midmap filters.
    //gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
    
    // Delete your loaded data
    free( data2 );


    // Return the texture.
    return _texture_1;

    
    
}

/*!
 This sets the texture blend model
 @param mode - the values 0,1, or 2
 @return true, when a new mode was set, false when current and new mode are equal
 */
bool GLMultiTexture::setTextureBlendMode(int mode)
{
    if(mode < 0 || mode > 2)return false;
    if(_texture_blend_mode == mode)return false;
    
    // write the texture blend mode
    _texture_blend_mode = mode;
    
    _dirty = true;
    
    return true;

}




/*!
 Adds the variables of this object to the shader program
 */
//virtual
bool GLMultiTexture::addVariablesToProgram(GLuint program, int variable_index)
{
    if(program == -1)return false; // no program exits
    
    
    GLint params;
    glGetProgramiv( program, GL_LINK_STATUS, &params);
    if(params == GL_FALSE)
    {
        cerr << "[GLTexture] Program " << program << " has not been linked. Textures cannot be added." << endl;
        return false;
    }
    
    // enable the program
    glUseProgram(program);
    
    
    // get the location of a uniform variable. Note, the program must be linked at this position.
    // location of the texture in the glsl program
    _textureIdx1 = glGetUniformLocation(program, _glsl_names[0].c_str() );
    checkUniform(_textureIdx1, _glsl_names[0]);
    
    
    _textureIdx2 = glGetUniformLocation(program, _glsl_names[1].c_str() );
    checkUniform(_textureIdx2, _glsl_names[1]);
    
    
    _textureBlendModelIdx = glGetUniformLocation(program, _glsl_names[2].c_str() );
    checkUniform(_textureBlendModelIdx, _glsl_names[2]);
    
    //****************************************************************************************************
    // Link the texture to the uniform variable and texture unit 0;
    
    /*
     glActiveTexture tells OpenGL which texture unit we want to use. GL_TEXTURE0 is the first texture unit, so we will just use that.
     */
    glActiveTexture(GL_TEXTURE0);
    
    
    //We use glBindTexture bind our texture into the active texture unit.
    glBindTexture(GL_TEXTURE_2D, _texture_1);
    
    /*
     Then we set the tex uniform of the shaders to the index of the texture unit. We used texture unit zero, so we set the tex uniform to the integer value 0.
     */
    glUniform1i(_textureIdx1, 0);
    
    
    
    
    /*
     glActiveTexture tells OpenGL which texture unit we want to use. GL_TEXTURE0 is the first texture unit, so we will just use that.
     */
    glActiveTexture(GL_TEXTURE1);
    
    
    //We use glBindTexture bind our texture into the active texture unit.
    glBindTexture(GL_TEXTURE_2D, _texture_2);
    
    /*
     Then we set the tex uniform of the shaders to the index of the texture unit. We used texture unit zero, so we set the tex uniform to the integer value 0.
     */
    glUniform1i(_textureIdx2, 1);
    
    
    
    
    // update the variable
    dirty(program);
    
    return true;

}


/*!
 The function indicates that the variables of this object require an update
 */
//virtual
bool GLMultiTexture::dirty(GLuint program)
{
    // enable the program
    glUseProgram(program);
    
    // write the texture blend mode
    glUniform1i(_textureBlendModelIdx, _texture_blend_mode);
    
    
    // disable the program
    glUseProgram(0);
    
    _dirty = false;
    
    return true;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Midmap texture

GLMipMapTexture::GLMipMapTexture()
{
    _textureIdx = -1;
    _texture_blend_mode = 0;
    _dirty = false;
}


GLMipMapTexture::~GLMipMapTexture()
{

}

/*!
 Loads a texture from a file and creates the necessary texture objects
 @param path_and_file to the texture object
 @return int - the texture id when the texture was sucessfully loaded.
 */
int GLMipMapTexture::loadAndCreateTexture(string path_and_file)
{
    
    unsigned int channels;
    unsigned int width;
    unsigned int height;
    
    // Load the texture from file.
    unsigned char* data = loadBitmapFile(path_and_file, channels, width,  height );
    if(data == NULL )return -1;

    
    //**********************************************************************************************
    // Texture generation
    
    glActiveTexture(GL_TEXTURE0);
    
    // Generate a texture, this function allocates the memory and
    // associates the texture with a variable.
    glGenTextures(1, &_texture );
    
    // Set a texture as active texture.
    glBindTexture( GL_TEXTURE_2D, _texture );

    //------------------------------------------------------------------------------------------------
    // Change the parameters of your texture units.
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );

    // TRY THESE TWO PARAMETERS. They change the mipmap mode from linear to nearest
    // glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST );
    // glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST );
    //------------------------------------------------------------------------------------------------
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    
    
    //**********************************************************************************************
    // Create a midmap texture pyramid and load it to the graphics hardware.
    // Note, the MIN and MAG filter must be set to one of the available midmap filters.
	if(channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    else if(channels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	// This generates the midmaps
	glGenerateMipmap(GL_TEXTURE_2D);

    
    // Delete your loaded data
    free( data );
    
    // Return the texture.
    return _texture;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 Load a bitmap image from a file
 @param path_and_file - the path and filename 
 @param data - a pointer to return the data. Note, the data object should be deleted once the data is not required anymore. 
 @param channels - reference to an integer to keep the channels. 
 @param width, height, - reference to integers to return the dimensions in pixels.
*/
unsigned char* loadBitmapFile(string path_and_file, unsigned int& channels, unsigned int& width, unsigned int& height )
{

    // Check whether we load a bitmap file
    
    int idx = path_and_file.find_last_of(".");
    string suffix = path_and_file.substr(idx+1, 3);
    
    if (suffix.compare("bmp") != 0) {
        cerr << "[ERROR] Filetype " << suffix << " is currently not supported. This example only support bitmap files. " << endl;
        return NULL;
    }
    
	string new_path_and_file;
	SearchTexture(path_and_file, new_path_and_file);

    
    //**********************************************************************************************
    // Loads the file content
    
    channels = 3;
    unsigned char* data;
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    //unsigned int width, height;
    unsigned int imageSize;
    
    // This opens a file
    FILE * file;
    file = fopen( new_path_and_file.c_str(), "rb" );
    
    if ( file == NULL ) return 0;
    
    // This reads the header of the file and checks the length.
    if ( fread(header, 1, 54, file)!=54 )
    {
        // If not 54 bytes read, this is not a bmp.
        // Only a bmp has a header of length 54
        printf("Not a correct BMP file\n");
        return NULL;
    }
    
    // Read the start position of the data, the size, the width, and height.
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    channels = imageSize / (width * height);
    
    // Create memory for this texture
    data = (unsigned char *)malloc( width * height * channels );
    
    // Read the data from a file.
    fread( data, width * height * channels, 1, file );
    
    // Release the file.
    fclose( file );
    
    
    return data;

}




/*!
 Verifies wheterh a file [name] exits
 @param name - the path and the name of the file.
 */
bool SearchTexture (const std::string& name, string& new_name)
{
	new_name = "";

	bool exits = false;

    ifstream f(name.c_str());
    if (f.good()) {
        f.close();
		new_name = name;
        exits = true;
		return exits;
    } else {
        f.close();
        exits = false;
    }



	int idx = name.find_first_of("/");
    string newstring = name.substr(idx+1, name.length() - 3);


	ifstream f2(newstring.c_str());
    if (f2.good()) {
        f2.close();
        exits = true;
    } else {
        f2.close();
        exits = false;
    }


	if(exits)
	{
		new_name = newstring;
	}


	return exits;
}
