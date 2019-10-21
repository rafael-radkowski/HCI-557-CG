#include "BMPLoader.h"

#ifdef _WIN32
#else
#include <sys/stat.h>
#endif

using namespace cs557;

 /*
Load a bitmap image from a file
@path_and_file - string containing the relative or absolute location of the file.
@param width, height - pointers to locations to return the width and height in pixels. 
@param channels - destination to return the number of channels.
@param data - pointer to return the data. Note that the function allocates the memory. 
So you need to free it on your own later. 
*/
bool cs557::LoadBMPFromFile(const string path_and_file, int* width, int* height, int* channels,  unsigned char **data)
{

     // Check whether we load a bitmap file
    
    int idx = path_and_file.find_last_of(".");
    string suffix = path_and_file.substr(idx+1, 3);
    
    if (suffix.compare("bmp") != 0) {
        cerr << "[ERROR] Filetype " << suffix << " is currently not supported. This example only support bitmap files. " << endl;
        return false;
    }

    // Check whether the file exits
	string local_path_and_file = path_and_file;
	bool ret =  FileUtils::Search(path_and_file, local_path_and_file);
    
	if(!ret)
	{
		cerr << "[ERROR] Cannot find the file " << path_and_file << "." << endl;
        return false;
	}
    
    //**********************************************************************************************
    // Loads the file content
    
    (*channels) = 3;
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int imageSize;
    
    // This opens a file
    FILE * file;
    file = fopen( local_path_and_file.c_str(), "rb" );
    
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
    (*width)      = *(int*)&(header[0x12]);
    (*height)     = *(int*)&(header[0x16]);
    (*channels) = imageSize / ((*width) * (*height));
    
    int size = (*width) * (*height) * (*channels);

    // Create memory for this texture
    *data = (unsigned char *)malloc( size );
    
    // Read the data from a file.
    fread( *data, size, 1, file );

    return true;
}



/*!
 Verifies wheterh a file [name] exits
 @param name - the path and the name of the file.
 */
bool cs557::Exists (const std::string& name)
{
   #ifdef _WIN32
 #if defined(_MSC_VER) && (_MSC_VER >= 1916)	 
    if (std::filesystem::exists(name)) {
#else
	if (std::experimental::filesystem::exists(name)) {
#endif
        return true;
    } else {
		return false;
    }
    #else
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
    #endif
}

