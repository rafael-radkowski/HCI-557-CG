/*
This code is part of CS/CPRE/ME 557 - Computer Graphics

Datatype declaration for light and material.
The file declares two datatypes: Material and LightSources, 
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


#ifndef COMMONTYPES
#define COMMONTYPES


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

// The material parameters
uniform struct Material {
    vec3  diffColor;
 	float diffInt;
 	vec3  ambColor;
 	float ambInt;
 	vec3  specColor;
 	float specInt;
 	float shininess;
} mat[1];
			
The code will not work otherwise (or you have to adapt the names).

Note that the example shader code creates just one mat[1] object per program. 

*/
typedef struct Material
{

    // textrue id
	unsigned int texture_id;

    // model matrix
	glm::mat4 model_matrix;

    // Material
    glm::vec3 diffuse_mat;
    float     diffuse_int;
    glm::vec3 ambient_mat;
    float     ambient_int;
    glm::vec3 specular_mat;
    float     specular_int;
    float     specular_s;

    // error count, a helper to issue warning.
    int      error_count;

	Material(){
		texture_id = -1;
		model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 

        diffuse_mat = glm::vec3(0.8,0.8,0.8);
        diffuse_int = 1.0;
        ambient_mat = glm::vec3(0.8,0.8,0.8);
        ambient_int = 1.0;
        specular_mat = glm::vec3(0.8,0.8,0.8);
        specular_int = 1.0;
        specular_s = 12.0;

        error_count = 0;
	}


    /*
    The function passes all the uniform variables to the passed program.
    Note that the shader program must use the correct variable names.
    @param program_id - the shader program id as integer
    */
    inline void apply(int program_id ){
        setAllUniform( program_id );
    }


    /*
    The function passes all the uniform variables to the passed program.
    Note that the shader program must use the correct variable names.
    @param program_id - the shader program id as integer
    */
   [[deprecated("Replaced by apply. This function will be removed in the next future")]]
    inline void setAllUniform(int program_id )
    {

        glUseProgram(program_id );
        if(checkName(program_id, "mat[0].diffColor")) glUniform3fv(glGetUniformLocation(program_id , "mat[0].diffColor"), 1, &diffuse_mat[0]);
        if(checkName(program_id, "mat[0].diffInt")) glUniform1f(glGetUniformLocation(program_id , "mat[0].diffInt"), diffuse_int);
        if(checkName(program_id, "mat[0].ambColor")) glUniform3fv(glGetUniformLocation(program_id , "mat[0].ambColor"), 1, &ambient_mat[0]);
        if(checkName(program_id, "mat[0].ambInt")) glUniform1f(glGetUniformLocation(program_id , "mat[0].ambInt"), ambient_int);
        if(checkName(program_id, "mat[0].specColor")) glUniform3fv(glGetUniformLocation(program_id , "mat[0].specColor"), 1, &specular_mat[0]);
        if(checkName(program_id, "mat[0].specInt")) glUniform1f(glGetUniformLocation(program_id , "mat[0].specInt"), specular_int);
        if(checkName(program_id, "mat[0].shininess")) glUniform1f(glGetUniformLocation(program_id , "mat[0].shininess"), specular_s);

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
            std::cout << ret << " [ERROR] - Material - Cannot find shader program variable " << name << ".\nDid you add the right variable name?" << std::endl; 
            error_count++;
            return false;
        }
        return true;
    }


} Material;



/*
Enumeration to set the light type.
This information is passed to a shader program
and can be used to adapt the light and reflection model.
*/
typedef enum
{
    POINT,
    SPOT,
    DIRECT
} LightType;


/*
This struct is a helper which contains variables to store light soruce data.
Note that this struct expects to find a struct datatype in the glsl
shader program.
Use 

// The material parameters
uniform struct LightSource {
    vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float cutoff_in; // inner
	float cutoff_out; // outer
	float k1;    //attenuation
	float k2;    //attenuation
	bool used;
	int  type;  //0:point, 1:spot, 2:directional
} light[MAX_LIGHTS];

*/
typedef struct _LightSource
{

    // int light index
    // Set this values to a number 0-MAX_LIGHT
    // MAX_LIGHT is currently 10 in the example
    unsigned int    index;

    // position and direction
    glm::vec3 pos;
    glm::vec3 dir;

    // color of the light
    glm::vec3  color;
	
    // intensity of the light
    float intensity;

    // The example shader code creates 
    // 10 lights (MAX_LIGHTS) and memory for it.
    // The lights are only processed if the variable used==true.
    // Since the uniform variable is false by default, unused lights 
    // will not be processed. 
    bool       used;

    // spotlight
    float     cutoff_in; // inner cutoff
    float     cutoff_out; // outer cutoff

    // attenuation
    float     k1; // linear
    float     k2; // quadratic

    // type of the light
    LightType   type;

     // error count, a helper to issue warning.
    int      error_count;

	_LightSource(){
        index = -1;
        pos =  glm::vec3(0.0,0.0,0.0);
        dir =  glm::vec3(0.0,0.0,0.0);
        color =  glm::vec3(1.0,1.0,1.0);
        cutoff_in = 0.0;
        cutoff_out = 0.0;
        intensity = 1.0;
        k1 = 0.0;
        k2 = 0.0;
        type = POINT;
        used = true;

        error_count = 0;
	}

    /*
    The function passes all the uniform variables to the passed program.
    Note that the shader program must use the correct variable names.
    @param program_id - the shader program id as integer
    */
    inline void apply(int program_id ){
        setAllUniform( program_id );
    }

    /*
    The function passes all the uniform variables to the passed program.
    Note that the shader program must use the correct variable names.
    @param program_id - the shader program id as integer
    */
   [[deprecated("Replaced by apply. This function will be removed in the next future")]]
    inline void setAllUniform(int shader_program_id)
    {
        glUseProgram(shader_program_id );
        if(checkName(shader_program_id, getVariableName("light", index, "position") )) glUniform3fv(glGetUniformLocation(shader_program_id , getVariableName("light", index, "position").c_str()), 1, &pos[0]);
        if(checkName(shader_program_id, getVariableName("light", index, "direction") )) glUniform3fv(glGetUniformLocation(shader_program_id , getVariableName("light", index, "direction").c_str()), 1, &dir[0]);
        if(checkName(shader_program_id, getVariableName("light", index, "color") )) glUniform3fv(glGetUniformLocation(shader_program_id , getVariableName("light", index, "color").c_str()), 1, &color[0]);
        if(checkName(shader_program_id, getVariableName("light", index, "intensity") )) glUniform1f(glGetUniformLocation(shader_program_id , getVariableName("light", index, "intensity").c_str()), intensity);
        if(checkName(shader_program_id, getVariableName("light", index, "used") )) glUniform1i(glGetUniformLocation(shader_program_id , getVariableName("light", index, "used").c_str()), (int)used);
        if(checkName(shader_program_id, getVariableName("light", index, "type") )) glUniform1i(glGetUniformLocation(shader_program_id , getVariableName("light", index, "type").c_str()), (int)type);
        if(checkName(shader_program_id, getVariableName("light", index, "cutoff_in") )) glUniform1f(glGetUniformLocation(shader_program_id , getVariableName("light", index, "cutoff_in").c_str()), cutoff_in);
        if(checkName(shader_program_id, getVariableName("light", index, "cutoff_out") )) glUniform1f(glGetUniformLocation(shader_program_id , getVariableName("light", index, "cutoff_out").c_str()), cutoff_out);
        if(checkName(shader_program_id, getVariableName("light", index, "k1") )) glUniform1f(glGetUniformLocation(shader_program_id , getVariableName("light", index, "k1").c_str()), k1);
        if(checkName(shader_program_id, getVariableName("light", index, "k2") )) glUniform1f(glGetUniformLocation(shader_program_id , getVariableName("light", index, "k2").c_str()), k2);
        glUseProgram(0);
    }


    /*
    This function checks for the variable names in the shader program shader_program_id
    */
    inline bool checkName(int shader_program_id, std::string variable_name)
    {

        int ret = glGetUniformLocation(shader_program_id, variable_name.c_str());
        if(ret == -1){
           // std::cout << ret << " [ERROR] - LightSource " << index << "  - Cannot find shader program variable " << variable_name << " (program: "<< shader_program_id << ").\nDid you add the right variable name?" << std::endl; 
            return false;
        }
        return true;
    }


    /*
    Assemble a varibale name string from three components.
    */
    inline std::string getVariableName(std::string struct_name, int index, std::string variable_name)
    {
        std::string name = struct_name;
        name.append("[");
        name.append(std::to_string(index));
        name.append("].");
        name.append(variable_name);
        return name;
    }

}LightSource;

}//namespace cs557

#endif