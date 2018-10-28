#include "Texture2D.h"




 /*
Create a 2D texture. 
@param width, height - the width and the height of the texture in pixels.
@param channels - the number of channels.
@param data - a pointer to the data. 
@param dst_texture_id -  a pointer to return the texture identifer
@return true if successfull
*/
bool cs557::CreateTexture2D(int width, int height, int channels, unsigned char* data, unsigned int* dst_texture_id,
     int mode, int texture_unit )
{
    // texture unity
    glActiveTexture(texture_unit); //GL_TEXTURE0

     // Generate a texture, this function allocates the memory and
    // associates the texture with a variable.
    glGenTextures(1, dst_texture_id );
    
    // Set a texture as active texture.
    glBindTexture( GL_TEXTURE_2D, *dst_texture_id );


    
    // Change the parameters of your texture units.
    //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_BLEND );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, mode );
    
    
    // Create a texture and load it to your graphics hardware. This texture is automatically associated
    // with texture 0 and the textuer variable "texture" / the active texture.
    if(channels == 3){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    }
    else if(channels == 4){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    }
    
    //**********************************************************************************************
    // Create a midmap texture pyramid and load it to the graphics hardware.
    // Note, the MIN and MAG filter must be set to one of the available midmap filters.
    //gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
    

    return true;
}




 /*
Create a 2D texture. 
@param file_and_path - relative or absolute path pointing to the texture
@param dst_texture_id -  a pointer to return the texture identifer
@return true if successfull
*/
bool cs557::LoadAndCreateTexture2D(string file_and_path, unsigned int* dst_texture_id)
{

    unsigned char * g_data = NULL;
    int width = -1; 
    int height = -1; 
    int channels = -1;
    

    // Load the file
    bool ret = cs557::LoadBMPFromFile( file_and_path, &width, &height, &channels, &g_data);

    if(!ret)
    {
        std::cout << "[ERROR] - Did not load bmp file: " << file_and_path << "." << std::endl;
        return false;
    }

    // Create the texture
    ret = cs557::CreateTexture2D(width, height, channels, g_data, dst_texture_id);


    if(!ret)
    {
        std::cout << "[ERROR] - Did not create a texture for file: " << file_and_path << "." << std::endl;
        return false;
    }

    return true;
}



    /*
Create a single 2D texture and assign it to texture unit GL_TEXTURE0 
@param file_and_path - relative or absolute path pointing to the texture
@param dst_texture_id -  a pointer to return the texture identifer
@param texture_unit - a pointer to return the texture unit
@param wrap_mode - the GL wrapping mode. Use GL_REPEAT, GL_CLAMP_TO_BORDER, etc.
@return true if successfull
*/
bool cs557::LoadAndCreateTexture2D(string file_and_path, unsigned int* dst_texture_id, int wrap_mode, int texture_unit)
{
    unsigned char * g_data = NULL;
    int width = -1; 
    int height = -1; 
    int channels = -1;
    

    // Load the file
    bool ret = cs557::LoadBMPFromFile( file_and_path, &width, &height, &channels, &g_data);

    if(!ret)
    {
        std::cout << "[ERROR] - Did not load bmp file: " << file_and_path << "." << std::endl;
        return false;
    }

    // Create the texture
    ret = cs557::CreateTexture2D(width, height, channels, g_data, dst_texture_id, wrap_mode);


    if(!ret)
    {
        std::cout << "[ERROR] - Did not create a texture for file: " << file_and_path << "." << std::endl;
        return false;
    }

    return true;
    
}