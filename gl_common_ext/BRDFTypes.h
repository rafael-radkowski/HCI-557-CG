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
    float metallic; // metalic factor runs in a range from 0 to 1
 	float roughness; // roughness factor runs in a range from 0 to 1
 	float ao; // ambient intensity
    vec3  lightColor; // color of the light. 

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
        F0 = glm::vec3(0.56, 0.57, 0.58);
		
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


}//namespace cs557

#endif