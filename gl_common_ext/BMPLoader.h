/*


---------------------------------------------------------
Last edits:

Oct 21, 2019, RR
- Added FilesystemCheck.h and removed experimental/filesystem
- Added code for filesystem::exists() for the experimental version and the new version. 
*/
#pragma once


// stl include
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 

#include "FilesystemCheck.h"

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// glfw includes
#include <GLFW/glfw3.h>

// local
#include "FileUtils.h"


using namespace std;


namespace cs557
{
    /*
    Load a bitmap image from a file
    @path_and_file - string containing the relative or absolute location of the file.
    @param width, height - pointers to locations to return the width and height in pixels. 
    @param channels - destination to return the number of channels.
    @param data - pointer to return the data. Note that the function allocates the memory. 
    So you need to free it on your own later. 
    */
    bool LoadBMPFromFile(const string path_and_file, int* width, int* height, int* channels,  unsigned char **data);




}
