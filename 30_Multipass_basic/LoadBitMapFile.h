#pragma once

// stl include
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <experimental\filesystem>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


/*!
Load a bitmap image from a file
@param path_and_file - the path and filename
@param data - a pointer to return the data. Note, the data object should be deleted once the data is not required anymore.
@param channels - reference to an integer to keep the channels.
@param width, height, - reference to integers to return the dimensions in pixels.
*/
unsigned char* LoadBitMapFile(string path_and_file, unsigned int& channels, unsigned int& width, unsigned int& height)
{

	
	// Check whether we load a bitmap file

	int idx = path_and_file.find_last_of(".");
	string suffix = path_and_file.substr(idx + 1, 3);

	if (suffix.compare("bmp") != 0) {
		cerr << "[ERROR] Filetype " << suffix << " is currently not supported. This example only support bitmap files. " << endl;
		return NULL;
	}


	if (!experimental::filesystem::exists(path_and_file)) {
		cerr << "[ERROR] File " << path_and_file << " does not exists. " << endl;
		return NULL;
	}


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
	file = fopen(path_and_file.c_str(), "rb");

	if (file == NULL) return 0;

	// This reads the header of the file and checks the length.
	if (fread(header, 1, 54, file) != 54)
	{
		// If not 54 bytes read, this is not a bmp.
		// Only a bmp has a header of length 54
		printf("Not a correct BMP file\n");
		return NULL;
	}

	// Read the start position of the data, the size, the width, and height.
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	channels = imageSize / (width * height);

	// Create memory for this texture
	data = (unsigned char *)malloc(width * height * channels);

	// Read the data from a file.
	fread(data, width * height * channels, 1, file);

	// Release the file.
	fclose(file);


	return data;

}

