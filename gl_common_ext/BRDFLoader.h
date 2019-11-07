

#ifndef _BRDFLOADER_
#define _BRDFLOADER_


#include <iostream>
#include <string>


#include "BRDFTypes.h" // The brdf material
#include "BMPLoader.h"
#include "FileUtils.h"
#include "Texture2D.h"

using namespace std;

namespace cs557
{

class BRDFLoader
{
public:

    /*
    Read the BMP maps from and image and prepare the textures
    */
    static int ReadBMP(BRDFTexMaterial& mat, const string albedo_map, const string metalic_map, const string roughness_map, const string ambient_occlusion_map );



    static int ReadIBL(BRDFTexMaterial& mat, const string albedo_map, const string metalic_map, const string roughness_map, const string ambient_occlusion_map, const string ibl_map );

};


}
#endif