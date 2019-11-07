#include "BRDFLoader.h"


using namespace cs557;

/*
Read the BMP maps from and image and prepare the textures
*/
//static 
int BRDFLoader::ReadBMP(BRDFTexMaterial& mat, const string albedo_map, const string metalic_map, const string roughness_map, const string ambient_occlusion_map )
{
    int num = 0;

    // albedo 

    string albedo_map_;
    if (!FileUtils::Search(albedo_map, albedo_map_))
    {
        cout << "[BRDFLoader] - ERROR: Did not find the albedo map " << albedo_map << "." << endl;
        return -1;
    }

    bool ret = LoadAndCreateTexture2D( albedo_map_, &mat.albedoMapId, GL_CLAMP_TO_BORDER, GL_TEXTURE0);

    if(!ret){
        cout << "[BRDFLoader] - ERROR: Did not load the albedo map " << albedo_map << "." << endl;
        return -1;
    }
    
    num++;


    // metallic map 

    string metallic_map_;
    if (!FileUtils::Search(metalic_map, metallic_map_))
    {
        cout << "[BRDFLoader] - ERROR: Did not find the albedo map " << metalic_map << "." << endl;
        return -1;
    }

    ret = LoadAndCreateTexture2D( metallic_map_, &mat.metallicMapId, GL_CLAMP_TO_BORDER, GL_TEXTURE1);

    if(!ret){
        cout << "[BRDFLoader] - ERROR: Did not load the albedo map " << metalic_map << "." << endl;
        return -1;
    }
    num++;


    // roughness map 


    string roughness_map_;
    if (!FileUtils::Search(roughness_map, roughness_map_))
    {
        cout << "[BRDFLoader] - ERROR: Did not find the albedo map " << roughness_map << "." << endl;
        return -1;
    }

    ret = LoadAndCreateTexture2D( roughness_map_, &mat.roughnessMapId, GL_CLAMP_TO_BORDER, GL_TEXTURE2);

    if(!ret){
        cout << "[BRDFLoader] - ERROR: Did not load the albedo map " << roughness_map << "." << endl;
        return -1;
    }
    num++;



     // ambient occlusion map 


    string ambient_o_map_;
    if (!FileUtils::Search(ambient_occlusion_map, ambient_o_map_))
    {
        cout << "[BRDFLoader] - ERROR: Did not find the albedo map " << ambient_occlusion_map << "." << endl;
        return -1;
    }

    ret = LoadAndCreateTexture2D( ambient_o_map_, &mat.aoMapId, GL_CLAMP_TO_BORDER, GL_TEXTURE3);

    if(!ret){
        cout << "[BRDFLoader] - ERROR: Did not load the albedo map " << ambient_occlusion_map << "." << endl;
        return -1;
    }
    num++;

    return num;
}




 //static 
 int BRDFLoader::ReadIBL(BRDFTexMaterial& mat, const string albedo_map, const string metalic_map, const string roughness_map, const string ambient_occlusion_map, const string ibl_map )
 {
    // read the brdf images
    int ret = ReadBMP( mat, albedo_map,  metalic_map, roughness_map, ambient_occlusion_map );

    LoadAndCreateTexture2D(ibl_map, &mat.normalMapId, GL_CLAMP_TO_BORDER,  GL_TEXTURE4);

    return ++ret;
 }
