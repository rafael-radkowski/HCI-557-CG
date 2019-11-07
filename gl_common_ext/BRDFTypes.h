/*
This code is part of CS/CPRE/ME 557 - Computer Graphics

Datatype declaration for a BRDF material.
The file declares one datatype: BRDFMaterial, 
which can be used to store material and light parameters, obviously. 


Rafael Radkowski
Iowa State University
November 2018
rafael@iastate.edu

MIT License
-----------------------------------------------------
Last edits:

Nov. 5th, 2019, RR
- Replace setAllUniform with apply and marked the previous function as deprecated. 


*/


#ifndef BRDFTYPES
#define BRDFTYPES


// stl include
#include <iostream>
#include <string>
#include <strstream>
#include <sstream>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions



namespace cs557{


/*
This struct is a helper which contains material data. 
Note that this struct expects to find a struct datatype in the glsl
shader program for the material

Use

// The parameters for a BRDF materail
uniform struct BRDFMaterial {
    vec3 albedo; // the general diffuse color. 
    float metallic; // metalic factor runs in a range from 0 to 1
 	float roughness; // roughness factor runs in a range from 0 to 1
 	float ao; // ambient intensity
    vec3  lightColor; // color of the light. 
    gvec3  F0;

    float k1; // linear attenuation
    float k2; // quadratic attenuation
} brdf[1];

			
The code will not work otherwise (or you have to adapt the names).

Note that the example shader code creates just one mat[1] object per program. 

*/
typedef struct BRDFMaterial
{
    glm::vec3 albedo; // the general diffuse color. 
    float metallic; // metalic factor runs in a range from 0 to 1
 	float roughness; // roughness factor runs in a range from 0 to 1
 	float ao; // ambient intensity
    glm::vec3  lightColor; // color of the light. 
    glm::vec3  F0;

    float k1; // linear attenuation
    float k2; // quadratic attenuation


    // error count, a helper to issue warning.
    int      error_count;

	BRDFMaterial(){
        albedo = glm::vec3(1.0,0.0,0.0);
		metallic = 0.5;
		roughness = 0.5;
		ao = 1.0;
        //F0 = glm::vec3(0.05, 0.05, 0.05);
        F0 = glm::vec3(0.91, 0.92, 0.92);
		
		lightColor = glm::vec3(23.47, 21.31, 20.79);
		k1 = 0.1;
		k2 = 0.0;
		
        error_count = 0;
	}


    /*
    The function passes all the uniform variables to the passed program.
    Note that the shader program must use the correct variable names.
    @param program_id - the shader program id as integer
    */
    inline void apply(int program_id )
    {

        glUseProgram(program_id );
        if(checkName(program_id, "brdf[0].albedo")) glUniform3fv(glGetUniformLocation(program_id , "brdf[0].albedo"), 1,&albedo[0]);
        if(checkName(program_id, "brdf[0].metallic")) glUniform1f(glGetUniformLocation(program_id , "brdf[0].metallic"), metallic);
        if(checkName(program_id, "brdf[0].roughness")) glUniform1f(glGetUniformLocation(program_id , "brdf[0].roughness"), roughness);
        if(checkName(program_id, "brdf[0].ao")) glUniform1f(glGetUniformLocation(program_id , "brdf[0].ao"), ao);
        if(checkName(program_id, "brdf[0].F0")) glUniform3fv(glGetUniformLocation(program_id , "brdf[0].F0"), 1,  &F0[0]);
        if(checkName(program_id, "brdf[0].lightColor")) glUniform3fv(glGetUniformLocation(program_id , "brdf[0].lightColor"), 1,&lightColor[0]);
        if(checkName(program_id, "brdf[0].k1")) glUniform1f(glGetUniformLocation(program_id , "brdf[0].k1"), k1);
        if(checkName(program_id, "brdf[0].k2")) glUniform1f(glGetUniformLocation(program_id , "brdf[0].k2"), k2);

        glUseProgram(0);
    }


    /*
    This function checks for the variable names in the shader program shader_program_id
    @param program_id - the shader program id as integer.
    @param name - the variable name
    @return true - if the name exists, false otherwise. 
    */
    inline bool checkName(int program_id, std::string name)
    {
        
        int ret = glGetUniformLocation(program_id, name.c_str());
        if(ret == -1 && error_count < 7){
            std::cout << ret << " [ERROR] - BRDFMaterial - Cannot find shader program variable " << name << ".\nDid you add the right variable name?" << std::endl; 
            error_count++;
            return false;
        }
        return true;
    }


} BRDFMaterial;







/*
This struct is a helper which contains material data. 
Note that this struct expects to find a struct datatype in the glsl
shader program for the material

Use

// The parameters for a BRDF materail
uniform struct BRDFTexMaterial {
    uniform sampler2D albedoMap;
    uniform sampler2D normalMap;
    uniform sampler2D metallicMap;
    uniform sampler2D roughnessMap;
    uniform sampler2D aoMap;


    vec3  lightColor; // color of the light. 
    float k1; // linear attenuation
    float k2; // quadratic attenuation
} brdf[1];

			
The code will not work otherwise (or you have to adapt the names).

Note that the example shader code creates just one mat[1] object per program. 

*/
typedef struct BRDFTexMaterial
{
    unsigned int albedoMapId;
    unsigned int normalMapId;
    unsigned int metallicMapId;
    unsigned int roughnessMapId;
    unsigned int aoMapId;

    glm::vec3  F0;
    glm::vec3  lightColor; // color of the light. 
    float k1; // linear attenuation
    float k2; // quadratic attenuation


    // error count, a helper to issue warning.
    int      error_count;

	BRDFTexMaterial(){
        albedoMapId = 0;
        normalMapId = 0;
        metallicMapId = 0;
        roughnessMapId = 0;
        aoMapId = 0;

        F0 = glm::vec3(0.08, 0.08, 0.08);		
        //F0 = glm::vec3(0.91, 0.92, 0.92);
		lightColor = glm::vec3(23.47, 21.31, 20.79);
		k1 = 0.1;
		k2 = 0.0;

       
		
        error_count = 0;
	}


    /*
    The function passes all the uniform variables to the passed program.
    Note that the shader program must use the correct variable names.
    @param program_id - the shader program id as integer
    */
    inline void apply(int program_id )
    {

        glUseProgram(program_id );
        glEnable(GL_TEXTURE_2D);
        if(checkName(program_id, "brdf_tex[0].albedoMap")) glUniform1i(glGetUniformLocation(program_id , "brdf_tex[0].albedoMap"), 0);
        if(checkName(program_id, "brdf_tex[0].metallicMap")) glUniform1i(glGetUniformLocation(program_id , "brdf_tex[0].metallicMap"), 1);
        if(checkName(program_id, "brdf_tex[0].roughnessMap")) glUniform1i(glGetUniformLocation(program_id , "brdf_tex[0].roughnessMap"), 2);
        if(checkName(program_id, "brdf_tex[0].aoMap")) glUniform1i(glGetUniformLocation(program_id , "brdf_tex[0].aoMap"), 3);
        if(checkName(program_id, "brdf_tex[0].normalMap")) glUniform1i(glGetUniformLocation(program_id , "brdf_tex[0].normalMap"), 4);
        
        if(checkName(program_id, "brdf_tex[0].F0")) glUniform3fv(glGetUniformLocation(program_id , "brdf_tex[0].F0"), 1,  &F0[0]);
        if(checkName(program_id, "brdf_tex[0].lightColor")) glUniform3fv(glGetUniformLocation(program_id , "brdf_tex[0].lightColor"), 1,&lightColor[0]);
        if(checkName(program_id, "brdf_tex[0].k1")) glUniform1f(glGetUniformLocation(program_id , "brdf_tex[0].k1"), k1);
        if(checkName(program_id, "brdf_tex[0].k2")) glUniform1f(glGetUniformLocation(program_id , "brdf_tex[0].k2"), k2);

        glUseProgram(0);
    }


    /*
    This function checks for the variable names in the shader program shader_program_id
    @param program_id - the shader program id as integer.
    @param name - the variable name
    @return true - if the name exists, false otherwise. 
    */
    inline bool checkName(int program_id, std::string name)
    {
        
        int ret = glGetUniformLocation(program_id, name.c_str());
        if(ret == -1 && error_count < 7){
            std::cout << ret << " [ERROR] - BRDFMaterial - Cannot find shader program variable " << name << ".\nDid you add the right variable name?" << std::endl; 
            error_count++;
            return false;
        }
        return true;
    }


    inline void activateMaps(int program_id){
        glUseProgram(program_id );
        glEnable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedoMapId);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, metallicMapId);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, roughnessMapId);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, aoMapId);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, normalMapId);
    }


} BRDFTexMaterial;



typedef struct BRDFIBLMaterial
{

    BRDFTexMaterial texture_material;
    unsigned int iblMapId;


    glm::vec3  F0;
    glm::vec3  lightColor; // color of the light. 
    float k1; // linear attenuation
    float k2; // quadratic attenuation


    BRDFIBLMaterial()
    {
        // error count, a helper to issue warning.
        int      error_count;
    
        F0 = glm::vec3(0.08, 0.08, 0.08);		
        //F0 = glm::vec3(0.91, 0.92, 0.92);
		lightColor = glm::vec3(23.47, 21.31, 20.79);
		k1 = 0.1;
		k2 = 0.0;

        error_count = 0;

    }


}BRDFIBLMaterial;


}//namespace cs557

#endif